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

#ifndef _CEREBELLUM_CEREBELLUM_H_
# define _CEREBELLUM_CEREBELLUM_H_

# include "avahi/avahi-browser.h"
# include "avahi/avahi-client.h"
# include "loop/loop.h"
# include "ssl/ssl-client.h"

struct s_cerebellum {
  struct s_browser *browser;
  struct s_client *client;
  struct s_ssl_client *connection;
  struct s_loop *loop;
};

/**
 * @brief Allocate a specific cerebellum instance
 * @return a valid pointer on success, NULL on error
 */
struct s_cerebellum *s_cerebellum_new(void);

/**
 * @brief Deallocate a specific cerebellum instance
 * @param [in] cerebellum: cerebellum instance to delete
 */
void s_cerebellum_free(struct s_cerebellum *cerebellum);

/**
 * @brief Run a specific cerebellum
 * @param [in] cerebellum: cerebellum to run
 * @return 0 on success, an -errno value on error
 */
int s_cerebellum_run(struct s_cerebellum *cerebellum);

/**
 * @brief Quit a specific cerebellum
 * @param [in] cerebellum: cerebellum to quit
 * @return 0 on success, an -errno value on error
 */
int s_cerebellum_quit(struct s_cerebellum *cerebellum);

/**
 * @brief Get client behavior function
 * @return a valid pointer on success
 */
const struct s_client_funcs *s_cerebellum_client_get_funcs(void);

/**
 * @brief Get the browser behavior function
 * @return a valid pointer on success
 */
const struct s_browser_funcs *s_cerebellum_browser_get_funcs(void);

/**
 * @brief Get the ssl behavior function
 * @return a valid pointer on success
 */
const struct s_ssl_funcs *s_cerebellum_ssl_get_funcs(void);

#endif /* !_CEREBELLUM_CEREBELLUM_H_ */
