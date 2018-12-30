/*
 * This file is part of mytank-module.
 *
 * mytank-module is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mytank-module is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mytank-module.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <event.h>
#include <event2/event.h>
#include <event2/bufferevent_ssl.h>
#include <netinet/in.h>
#include <stdint.h>

#include "alloc.h"
#include "cond.h"
#include "log.h"
#include "ssl/ssl-client.h"

struct s_ssl_client {
  struct s_ssl_funcs funcs;
  struct s_loop *loop;
  char *name;

  struct {
    struct bufferevent *buffer;
    SSL_CTX *context;
  } ssl;

  void *userdata;
};

/**
 * @brief Generate a packet instance from the data gather inside the buffer
 * @param [in] buffer: buffer concerned by that process
 * @return a valid pointer on success, NULL on error
 */
static struct s_ssl_packet *_s_ssl_packet_generate(struct bufferevent *buffer)
{
  x_return_val_if_fail(buffer, NULL);

  struct evbuffer *event = evbuffer_new();
  x_return_val_if_fail(bufferevent_read_buffer(buffer, event) == 0, NULL);

  struct s_ssl_packet *packet = s_ssl_packet_new(evbuffer_pullup(event, -1),
    evbuffer_get_length(event));

  evbuffer_free(event);
  return packet;
}

/**
 * @brief Read callback for a bufferevent.
 * The read callback is triggered when new data arrives in the input buffer and
 * the amount of readable data exceed the low watermark which is 0 by default.
 * @param [in] buffer: buffer to read
 * @param [in] client: ssl client representation
 */
static void _s_ssl_client_read(struct bufferevent *buffer,
  struct s_ssl_client *client)
{
  x_return_if_fail(buffer);
  x_return_if_fail(client);

  if (client->funcs.read) {
    struct s_ssl_packet *packet = _s_ssl_packet_generate(buffer);
    client->funcs.read(client->userdata, packet);
    s_ssl_packet_free(packet);
  }
}

/**
 * @brief An event/error callback for a bufferevent.
 * The event callback is triggered if either an EOF condition or another
 * unrecoverable error was encountered.
 * @param [in] buffer: the bufferevent for which the error condition was reached
 * @param [in] what: a conjunction of flags: BEV_EVENT_READING or
 * BEV_EVENT_WRITING to indicate if the error was encountered on the read or
 * write path, and one of the following flags: BEV_EVENT_EOF, BEV_EVENT_ERROR,
 * BEV_EVENT_TIMEOUT, BEV_EVENT_CONNECTED.
 * @param [in] client: ssl client representation
 */
static void _s_ssl_client_event(struct bufferevent *buffer, short what,
  struct s_ssl_client *client)
{
  x_return_if_fail(buffer);
  x_return_if_fail(client);

  if ((what & BEV_EVENT_EOF) == BEV_EVENT_EOF) {
    client->funcs.connection(client->userdata, e_ssl_connection_close);
  } else if ((what & BEV_EVENT_TIMEOUT) == BEV_EVENT_TIMEOUT) {
    client->funcs.connection(client->userdata, e_ssl_connection_timeout);
  } else if ((what & BEV_EVENT_CONNECTED) == BEV_EVENT_CONNECTED) {
    client->funcs.connection(client->userdata, e_ssl_connection_connected);
  } else {
    int err = bufferevent_get_openssl_error(buffer);
    enum e_ssl_error error = (what & BEV_EVENT_READING) == BEV_EVENT_READING ?
      e_ssl_error_read :
      (what & BEV_EVENT_WRITING) == BEV_EVENT_WRITING ?
        e_ssl_error_write :
        e_ssl_error_connection;

    if (e_ssl_error_connection == error) {
      client->funcs.error(client->userdata, error, err, NULL);
    } else {
      struct s_ssl_packet *packet = _s_ssl_packet_generate(buffer);
      client->funcs.error(client->userdata, error, err, packet);
      s_ssl_packet_free(packet);
    }
  }
}

struct s_ssl_client *s_ssl_client_new(struct s_loop *loop,
  const struct s_ssl_funcs *funcs, void *userdata)
{
  x_return_val_if_fail(loop, NULL);
  x_return_val_if_fail(funcs, NULL);
  x_return_val_if_fail(s_ssl_funcs_check(funcs) == 0, NULL);

  struct s_ssl_client *client = x_malloc(sizeof(struct s_ssl_client));
  client->funcs = *funcs;
  client->loop = loop;
  client->name = strdup("unknown");
  client->userdata = userdata;
  return client;
}

void s_ssl_client_free(struct s_ssl_client *client)
{
  x_return_if_fail(client);

  if (client->ssl.context) {
    s_ssl_context_deinit();
    if (client->ssl.buffer) {
      SSL_set_shutdown(bufferevent_openssl_get_ssl(client->ssl.buffer),
      SSL_RECEIVED_SHUTDOWN);
      SSL_shutdown(bufferevent_openssl_get_ssl(client->ssl.buffer));
      bufferevent_free(client->ssl.buffer);
    }
    SSL_CTX_free(client->ssl.context);
  }
  x_free(client->name);
  x_free(client);
}

int s_ssl_client_connect(struct s_ssl_client *client,
  const char *certificate, const struct sockaddr_in *dest)
{
  x_return_val_if_fail(client, -EINVAL);
  x_return_val_if_fail(certificate, -EINVAL);
  x_return_val_if_fail(dest, -EINVAL);

  if (!client->ssl.buffer) {
    client->ssl.context = s_ssl_context_client_new(certificate);
    x_return_val_if_fail(client->ssl.context, -EBADE);

    SSL *ssl = SSL_new(client->ssl.context);
    uint32_t flags = BEV_OPT_DEFER_CALLBACKS | BEV_OPT_CLOSE_ON_FREE;
    client->ssl.buffer = bufferevent_openssl_socket_new(
      s_loop_tolibevent(client->loop), -1, ssl, BUFFEREVENT_SSL_CONNECTING,
      flags);
    x_return_val_if_fail(client->ssl.buffer, -EBADE);

    bufferevent_setcb(client->ssl.buffer,
      (bufferevent_data_cb)_s_ssl_client_read, NULL,
      (bufferevent_event_cb)_s_ssl_client_event, client);

    return bufferevent_socket_connect(client->ssl.buffer,
      (struct sockaddr *)dest, sizeof(*dest));
  }
  return 0;
}

int s_ssl_client_set_name(struct s_ssl_client *client, const char *name)
{
  x_return_val_if_fail(client, -EINVAL);
  x_return_val_if_fail(name, -EINVAL);

  x_free(client->name);
  client->name = strdup(name);
  return 0;
}

int s_ssl_client_write(struct s_ssl_client *client,
  const struct s_ssl_packet *packet)
{
  x_return_val_if_fail(client, -EINVAL);
  x_return_val_if_fail(packet, -EINVAL);

  return bufferevent_write(client->ssl.buffer, packet->payload, packet->size);
}
