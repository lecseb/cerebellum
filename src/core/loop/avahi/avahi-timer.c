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
#include "avahi-timer.h"
#include "cond.h"
#include "log.h"
#include "loop/loop.h"

struct s_avahi_timer {
  AvahiTimeoutCallback callback;
  struct event *event;
  void *userdata;
};

static void _s_avahi_timeout_cbk(x_unused evutil_socket_t fd,
  x_unused short e, struct s_avahi_timer *timer)
{
  x_return_if_fail(timer);

  timer->callback((AvahiTimeout *)timer, timer->userdata);
}

struct s_avahi_timer *s_avahi_timer_new(const AvahiPoll *api,
  const struct timeval *tv, AvahiTimeoutCallback callback, void *userdata)
{
  x_return_val_if_fail(api, NULL);
  x_return_val_if_fail(callback, NULL);

  struct s_loop *loop = api->userdata;

  struct s_avahi_timer *timer = x_malloc(sizeof(struct s_avahi_timer));
  timer->callback = callback;
  timer->userdata = userdata;

  struct timeval now, e_tv;
  timer->event = event_new(s_loop_tolibevent(loop), -1, EV_TIMEOUT,
    (event_callback_fn)_s_avahi_timeout_cbk, timer);
  if (!tv || ((tv->tv_sec == 0) && (tv->tv_usec == 0)))
    evutil_timerclear(&e_tv);
  else {
    (void)gettimeofday(&now, NULL);
    evutil_timersub(tv, &now, &e_tv);
  }
  if (!timer->event || evtimer_add(timer->event, &e_tv) < 0)
    goto error;

  return timer;

error:
  x_log_error("failed to allocate a timer\n");
  s_avahi_timer_free(timer);
  return NULL;
}

void s_avahi_timer_update(struct s_avahi_timer *timer,
  const struct timeval *tv)
{
  x_return_if_fail(timer);

  struct timeval now, e_tv;

  event_del(timer->event);

  if (!tv)
    return;

  (void)gettimeofday(&now, NULL);
  evutil_timersub(tv, &now, &e_tv);

  event_add(timer->event, &e_tv);
}

void s_avahi_timer_free(struct s_avahi_timer *timer)
{
  x_return_if_fail(timer);

  event_del(timer->event);
  event_free(timer->event);
  x_free(timer);
}
