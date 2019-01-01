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

#ifndef _DAEMON_CTX_H_
# define _DAEMON_CTX_H_

struct s_daemon_ctx {
  struct s_browser *browser;
  struct s_client *client;
  struct s_ssl_client *connection;
  struct event *event;
  struct s_loop *loop;
};

/**
 * @brief Allocate a new context for the daemon
 * @param [in] fd: daemon signal file descriptor
 * @return a valid pointer on success, NULL on error
 */
struct s_daemon_ctx *s_daemon_ctx_new(int fd);

/**
 * @brief Deallocate a specific context
 * @param [in] ctx: context to free
 */
void s_daemon_ctx_free(struct s_daemon_ctx *ctx);

/**
 * @brief Run the context
 * @param [in] ctx: context to run
 * @return 0 on success, an -errno value on error
 */
int s_daemon_ctx_run(struct s_daemon_ctx *ctx);

/**
 * @brief Quit the context
 * @param [in] ctx: context to quit
 * @return 0 on success, an -errno value on error
 */
int s_daemon_ctx_quit(struct s_daemon_ctx *ctx);

/**
 * @brief Get client behavior function
 * @return a valid pointer on success
 */
const struct s_client_funcs *s_daemon_ctx_client_get_funcs(void);

/**
 * @brief Get the browser behavior function
 * @return a valid pointer on success
 */
const struct s_browser_funcs *s_daemon_ctx_browser_get_funcs(void);

/**
 * @brief Get the ssl behavior function
 * @return a valid pointer on success
 */
const struct s_ssl_funcs *s_daemon_ctx_ssl_get_funcs(void);

#endif /* !_DAEMON_CTX_H_ */
