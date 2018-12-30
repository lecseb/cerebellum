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

#include <sys/signal.h>
#include "alloc.h"
#include "cond.h"
#include "log.h"
#include "loop-idle.h"
#include "loop/loop.h"

struct s_loop {
  struct event_base *base;
  struct event *signal;
  struct s_task_idle *idle;
};

struct s_loop *s_loop_new(void)
{
  struct s_loop *loop = x_malloc(sizeof(struct s_loop));
  loop->base = event_base_new();
  loop->idle = s_task_idle_new(loop);

  if (!loop->base || !loop->idle)
    goto error;

  return loop;

error:
  x_log_error("failed to allocate a loop\n");
  s_loop_free(loop);
  return NULL;
}

void s_loop_free(struct s_loop *loop)
{
  x_return_if_fail(loop);

  s_loop_quit(loop);

  s_task_idle_free(loop->idle);
  event_base_free(loop->base);
  x_free(loop);
}

int s_loop_run(struct s_loop *loop)
{
  x_return_val_if_fail(loop, -EINVAL);
  return event_base_loop(loop->base, 0);
}

int s_loop_quit(struct s_loop *loop)
{
  x_return_val_if_fail(loop, -EINVAL);

  return s_task_idle_wakeup(loop->idle);
}

struct event_base *s_loop_tolibevent(struct s_loop *loop)
{
  x_return_val_if_fail(loop, NULL);
  return loop->base;
}
