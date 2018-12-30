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
#include "api-alloc.h"
#include "api-cond.h"
#include "api-log.h"
#include "cerebrum.h"

/**
 * @brief Call after initialization when everything is done
 * @param [in] cerebrum: cerebrum instance
 */
static void _s_cerebrum_detection_running(struct s_cerebrum *cerebrum)
{
  api_return_if_fail(cerebrum);

  api_log_debug("cerebrum detection is running\n");
}

/**
 * @brief Call when the group name is already used and an alternative name
 * is needed. Allow the plugin to notify the change and apply it internaly
 * @param [in] cerebrum: cerebrum instance
 */
static void _s_cerebrum_detection_collision(struct s_cerebrum *cerebrum)
{
  api_return_if_fail(cerebrum);

  api_log_debug("cerebrum detection detected a collision\n");
}

/**
 * @brief Call when an error occured.
 * @param [in] cerebrum: cerebrum instance
 * @param [in] error: the errno value
 */
static void _s_cerebrum_detection_failure(struct s_cerebrum *cerebrum,
  int error)
{
  api_return_if_fail(cerebrum);

  api_log_error("cerebrum detection failed '%d'\n", error);
  s_cerebrum_quit(cerebrum);
}

const struct s_detection_funcs *s_cerebrum_detection_get_funcs(void)
{
  static const struct s_detection_funcs funcs = {
    .collision = (s_detection_collision_cbk)_s_cerebrum_detection_collision,
    .failure = (s_detection_failure_cbk)_s_cerebrum_detection_failure,
    .running = (s_detection_running_cbk)_s_cerebrum_detection_running,
  };
  return &funcs;
};
