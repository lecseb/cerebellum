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

#include <libdaemon/dlog.h>
#include <sys/eventfd.h>

#include "daemon-alloc.h"
#include "daemon-cond.h"
#include "daemon-ctx.h"
#include "avahi/avahi-browser.h"
#include "avahi/avahi-service.h"

/**
 * @brief Call after initialization when everything is done
 * @param [in] ctx: userdata passing through the allocation
 */
static void _s_daemon_ctx_client_running(struct s_daemon_ctx *ctx)
{
  daemon_return_if_fail(ctx);

  daemon_log(LOG_NOTICE, "daemon is running\n");

  struct s_service_data *data = s_service_generate();
  ctx->browser = s_browser_new(ctx->client, data,
    s_daemon_ctx_browser_get_funcs(), ctx);

  daemon_log(LOG_NOTICE, "daemon is ready\n");
}

/**
 * @brief Call when the group name is already used and an alternative name
 * is needed. Allow the plugin to notify the change and apply it internaly
 * @param [in] ctx: userdata passing through the allocation
 */
static void _s_daemon_ctx_client_collision(struct s_daemon_ctx *ctx)
{
  daemon_return_if_fail(ctx);

  daemon_log(LOG_NOTICE, "cerebellum client detected a collision\n");
}

/**
 * @brief Call when an error occured.
 * @param [in] ctx: userdata passing through the allocation
 * @param [in] error: the errno value
 */
static void _s_daemon_ctx_client_failure(struct s_daemon_ctx *ctx, int error)
{
  daemon_return_if_fail(ctx);

  daemon_log(LOG_ERR, "cerebellum client failed '%d'\n", error);
  s_daemon_ctx_quit(ctx);
}

const struct s_client_funcs *s_daemon_ctx_client_get_funcs(void)
{
  static const struct s_client_funcs funcs = {
    .collision = (s_client_collision_cbk)_s_daemon_ctx_client_collision,
    .failure = (s_client_failure_cbk)_s_daemon_ctx_client_failure,
    .running = (s_client_running_cbk)_s_daemon_ctx_client_running,
  };
  return &funcs;
};
