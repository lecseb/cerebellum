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

#include <arpa/inet.h>
#include <sys/eventfd.h>
#include "alloc.h"
#include "cerebellum.h"
#include "cond.h"
#include "log.h"

static const char *_g_cert_path = "/home/siroz/Project/mytank/certificate";

/**
 * @brief Call when an error occured.
 * @param [in] userdata: userdata passing through the allocation
 * @param [in] error: the errno value
 */
static void _s_cerebellum_failure(struct s_cerebellum *cerebellum, int error)
{
  x_return_if_fail(cerebellum);

  x_log_error("an error occured '%s'\n", strerror(error));
}

/**
 * @brief Call when a service is found
 * @param [in] userdata: userdata passing through the allocation
 * @param [in] domain: domain associated to the service
 * @param [in] address: address of the service
 * @param [in] port: port associated to the service
 * @param [in] txt: text data of the service
 */
static void _s_cerebellum_find(struct s_cerebellum *cerebellum,
  struct s_browser_data *data)
{
  x_return_if_fail(cerebellum);
  x_return_if_fail(data);

  x_log_debug("cerebrum '%s' found\n", data->name);

  struct sockaddr_in sin = { 0, };
  memset(&sin, '0', sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(8000);
  /* Convert IPv4 and IPv6 addresses from text to binary form */
  if (inet_pton(AF_INET, data->address, &sin.sin_addr) <= 0) {
    x_log_error("inet_pton failed\n");
    goto error;
  }

  cerebellum->connection = s_ssl_client_new(cerebellum->loop,
    s_cerebellum_ssl_get_funcs(), cerebellum);
  s_ssl_client_connect(cerebellum->connection, _g_cert_path, &sin);

error:
  s_browser_data_free(data);
  return;
}

/**
 * @brief Call when a service is removed
 * @param [in] userdata: userdata passing through the allocation
 * @param [in] data: service data to remove
 */
static void _s_cerebellum_remove(struct s_cerebellum *cerebellum,
  const struct s_browser_data *data)
{
  x_return_if_fail(cerebellum);
  x_return_if_fail(data);

  x_log_debug("service removed\n");
}

const struct s_browser_funcs *s_cerebellum_browser_get_funcs(void)
{
  static struct s_browser_funcs funcs = {
    .failure = (s_browser_failure_cbk)_s_cerebellum_failure,
    .find = (s_browser_find_cbk)_s_cerebellum_find,
    .remove = (s_browser_remove_cbk)_s_cerebellum_remove
  };
  return &funcs;
}
