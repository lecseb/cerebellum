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

#include <dbus/dbus.h>
#include <sys/signal.h>
#include "cerebellum.h"
#include "cond.h"
#include "log.h"

static struct s_cerebellum *_g_cerebellum;
static void _cerebellum_main_handler(x_unused int signal)
{
  s_cerebellum_quit(_g_cerebellum);
}

int main(void) {
  _g_cerebellum = s_cerebellum_new();
  x_return_val_if_fail(_g_cerebellum, -EBADE);

  x_return_val_if_fail(signal(SIGINT, _cerebellum_main_handler) != SIG_ERR,
    -EBADE);

  s_cerebellum_run(_g_cerebellum);

  s_cerebellum_free(_g_cerebellum);
  dbus_shutdown();
  return 0;
}
