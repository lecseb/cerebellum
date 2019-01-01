/*
 * This file is part of cerebellum.
 *
 * cerebellum is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * cerebellum is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with cerebellum.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _SSL_SSL_CLIENT_H_
# define _SSL_SSL_CLIENT_H_

# include <stdint.h>
# include <event2/event.h>
# include <netinet/in.h>

# include "daemon-loop.h"
# include "ssl/ssl.h"
# include "ssl/ssl-packet.h"

struct s_ssl_client;

/**
 * @brief Allocate a new ssl client
 * @param [in] loop: event loop base instance
 * @param [in] funcs: behavior callback functions
 * @param [in] userdata: userdata to use for @s_ssl_funcs callback
 * @return a valid pointer on success, NULL on error
 */
struct s_ssl_client *s_ssl_client_new(struct s_loop *loop,
  const struct s_ssl_funcs *funcs, void *userdata);

/**
 * @brief Deallocate a specific ssl communication socket
 * @param [in] client: client to delete
 */
void s_ssl_client_free(struct s_ssl_client *client);

/**
 * @brief Attempt a client connection on the address given in parameter
 * @param [in] client: client to connect
 * @param [in] certificate: certificate to authenticate
 * @param [in] dest: address and port information
 * @return 0 on success, an -errno value on error
 */
int s_ssl_client_connect(struct s_ssl_client *client,
  const char *certificate, const struct sockaddr_in *dest);

/**
 * @brief Set a name to the client interface
 * @param [in] client: client to modify
 * @param [in] name: name to set
 * @return 0 on success, an -errno value on error
 */
int s_ssl_client_set_name(struct s_ssl_client *client, const char *name);

/**
 * @brief Write a packet in the socket
 * @param [in] socket: socket concerned by the packet
 * @param [in] packet: payload received
 * @return 0 on success, an -errno value on error
 */
int s_ssl_client_write(struct s_ssl_client *client,
  const struct s_ssl_packet *packet);

#endif /* !_SSL_SSL_CLIENT_H_ */
