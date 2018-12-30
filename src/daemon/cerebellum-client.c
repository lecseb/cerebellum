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

#include <sys/eventfd.h>
#include "alloc.h"
#include "cerebellum.h"
#include "cond.h"
#include "log.h"
#include "avahi/avahi-service.h"

/**
 * @brief Call after initialization when everything is done
 * @param [in] cerebellum: cerebellum instance
 */
static void _s_cerebellum_client_running(struct s_cerebellum *cerebellum)
{
  x_return_if_fail(cerebellum);

  x_log_debug("cerebellum client is running\n");

  struct s_service_data *data = s_service_generate();
  cerebellum->browser = s_browser_new(cerebellum->client, data,
    s_cerebellum_browser_get_funcs(), cerebellum);

  x_log_debug("cerebellum is ready\n");
}

/**
 * @brief Call when the group name is already used and an alternative name
 * is needed. Allow the plugin to notify the change and apply it internaly
 * @param [in] cerebellum: cerebellum instance
 */
static void _s_cerebellum_client_collision(struct s_cerebellum *cerebellum)
{
  x_return_if_fail(cerebellum);

  x_log_debug("cerebellum client detected a collision\n");
}

/**
 * @brief Call when an error occured.
 * @param [in] cerebellum: cerebellum instance
 * @param [in] error: the errno value
 */
static void _s_cerebellum_client_failure(struct s_cerebellum *cerebellum,
  int error)
{
  x_return_if_fail(cerebellum);

  x_log_error("cerebellum client failed '%d'\n", error);
  s_cerebellum_quit(cerebellum);
}

const struct s_client_funcs *s_cerebellum_client_get_funcs(void)
{
  static const struct s_client_funcs funcs = {
    .collision = (s_client_collision_cbk)_s_cerebellum_client_collision,
    .failure = (s_client_failure_cbk)_s_cerebellum_client_failure,
    .running = (s_client_running_cbk)_s_cerebellum_client_running,
  };
  return &funcs;
};
