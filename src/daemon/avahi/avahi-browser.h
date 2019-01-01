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

#ifndef _AVAHI_AVAHI_BROWSER_H_
# define _AVAHI_AVAHI_BROWSER_H_

# include <avahi-client/client.h>
# include "avahi-client.h"
# include "avahi-service.h"

struct s_browser;

/**
 * @brief Cerebellum data description
 */
struct s_browser_data {
  char *address;
  char *domain;
  char *name;
  uint16_t port;
  char *txt;
  char *type;
};

/**
 * @brief Allocate a new browser data instance
 * @return a valid pointer on success, NULL on error
 */
struct s_browser_data *s_browser_data_new(const char *address,
  const char *domain, const char *name, uint16_t port, const char *txt,
  const char *type);

/**
 * @brief Deallocate a specific browser data instance
 * @return a valid pointer on success, NULL on error
 */
void s_browser_data_free(struct s_browser_data *data);

/**
 * @brief Call when a service is removed
 * @param [in] userdata: userdata passing through the allocation
 * @param [in] data: service data to remove
 */
typedef void (*s_browser_remove_cbk)(void *userdata,
  const struct s_browser_data *data);

/**
 * @brief Call when an error occured.
 * @param [in] userdata: userdata passing through the allocation
 * @param [in] error: the errno value
 */
typedef void (*s_browser_failure_cbk)(void *userdata, int error);

/**
 * @brief Call when a service is found
 * @param [in] userdata: userdata passing through the allocation
 * @param [in] domain: domain associated to the service
 * @param [in] address: address of the service
 * @param [in] port: port associated to the service
 * @param [in] txt: text data of the service
 */
typedef void (*s_browser_find_cbk)(void *userdata,
  struct s_browser_data *data);

struct s_browser_funcs {
  s_browser_failure_cbk failure;
  s_browser_find_cbk find;
  s_browser_remove_cbk remove;
};

/**
 * @brief Allocate a new group
 * @param [in] client: client structure
 * @param [in] data: service description
 * @param [in] funcs: functions behavior description
 * @param [in] userdata: user pointer
 * @return a valid pointer on success, NULL on error
 */
struct s_browser *s_browser_new(struct s_client *client,
  struct s_service_data *data, const struct s_browser_funcs *funcs,
  void *userdata);

/**
 * @brief Deallocate a specific group
 * @param [in] group: group to delete
 */
void s_browser_free(struct s_browser *browser);

#endif /* !_AVAHI_AVAHI_BROWSER_H_ */
