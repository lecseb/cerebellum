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

#include "cerebellum.h"

/**
 * @brief Connection status callback
 * @param [in] cerebellum: cerebellum instance
 * @param [in] state: current connection status
 */
static void _s_ssl_connection(struct s_cerebellum *cerebellum,
  enum e_ssl_connection state)
{
  x_return_if_fail(cerebellum);

  switch (state) {
  case e_ssl_connection_close:
    x_log_warning("ssl connection closed\n");
    break;
  case e_ssl_connection_connected:
    x_log_debug("ssl connection connected\n");
    break;
  case e_ssl_connection_timeout:
    x_log_warning("ssl connection timeout\n");
    break;
  }
}

/**
 * @brief Error status callback, called whenever a read / write operation
 * failed
 * @param [in] cerebellum: cerebellum instance
 * @param [in] type: error type definition
 * @param [in] packet: payload to send
 * @param [in] error: error received from ssl
 */
static void _s_ssl_error(struct s_cerebellum *cerebellum, enum e_ssl_error type,
  int error, const struct s_ssl_packet *packet)
{
  x_return_if_fail(cerebellum);

  (void)error;
  switch (type) {
  case e_ssl_error_connection:
    x_log_error("failed ssl connection\n");
    s_cerebellum_quit(cerebellum);
    break;
  case e_ssl_error_read:
    x_log_error("failed ssl read\n");
    x_return_if_fail(packet);
    break;
  case e_ssl_error_write:
    x_log_error("failed ssl write\n");
    x_return_if_fail(packet);
    break;
  default:
    x_log_warning("strange state... better to assert\n");
    x_assert(0, "swith case not handle");
  }
}

/**
 * @brief Read callback, called whenever a packet is received
 * @param [in] cerebellum: cerebellum instance
 * @param [in] packet: payload received
 */
static void _s_ssl_read(struct s_cerebellum *cerebellum,
  const struct s_ssl_packet *packet)
{
  x_return_if_fail(cerebellum);
  x_return_if_fail(packet);
}

const struct s_ssl_funcs *s_cerebellum_ssl_get_funcs(void)
{
  static const struct s_ssl_funcs funcs = {
    .connection = (s_ssl_connection_cbk)_s_ssl_connection,
    .error = (s_ssl_error_cbk)_s_ssl_error,
    .read = (s_ssl_read_cbk)_s_ssl_read,
  };
  return &funcs;
}
