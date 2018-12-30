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

#include "alloc.h"
#include "cond.h"
#include "log.h"
#include "cerebellum.h"

struct s_cerebellum *s_cerebellum_new(void)
{
  struct s_cerebellum *cerebellum = x_malloc(sizeof(struct s_cerebellum));
  cerebellum->loop = s_loop_new();
  cerebellum->client = s_client_new(s_loop_toavahi(cerebellum->loop),
    cerebellum, s_cerebellum_client_get_funcs());

  if (!cerebellum->client || !cerebellum->loop)
    goto error;

  return cerebellum;

error:
  x_log_error("failed to allocate a cerebellum\n");
  s_cerebellum_free(cerebellum);
  return NULL;
}

void s_cerebellum_free(struct s_cerebellum *cerebellum)
{
  x_return_if_fail(cerebellum);

  if (cerebellum->browser)
    s_browser_free(cerebellum->browser);
  if (cerebellum->connection)
    s_ssl_client_free(cerebellum->connection);

  s_client_free(cerebellum->client);
  s_loop_free(cerebellum->loop);
  x_free(cerebellum);
}

int s_cerebellum_run(struct s_cerebellum *cerebellum)
{
  x_return_val_if_fail(cerebellum, -EINVAL);

  return s_client_run(cerebellum->client) == 0 ?
    s_loop_run(cerebellum->loop) : -EBADE;
}

int s_cerebellum_quit(struct s_cerebellum *cerebellum)
{
  x_return_val_if_fail(cerebellum, -EINVAL);

  return s_loop_quit(cerebellum->loop);
}
