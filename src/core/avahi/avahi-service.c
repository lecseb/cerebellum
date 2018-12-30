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

#include <string.h>
#include <avahi-common/address.h>
#include "alloc.h"
#include "cond.h"
#include "log.h"
#include "avahi/avahi-service.h"

/**
 * @brief Key token used to identified cerebellum service description from the
 * rest
 */
const char *_g_service_key = "id=d6c4e9bcccdb8b16083036b45048dd52";

struct s_service_data *s_service_generate(void)
{
  struct s_service_data *data = x_malloc(sizeof(struct s_service_data));
  data->data = strdup(_g_service_key);
  data->domain = NULL;
  data->host = NULL;
  data->interface = AVAHI_IF_UNSPEC;
  data->name = strdup("cerebellum");
  data->port = 651;
  data->protocol = AVAHI_PROTO_INET;
  data->type = strdup("_http._tcp");
  return data;
}

void s_service_free(struct s_service_data *data)
{
  x_return_if_fail(data);

  if (data->name)
    x_free(data->name);
  if (data->type)
    x_free(data->type);
  if (data->domain)
    x_free(data->domain);
  if (data->host)
    x_free(data->host);
  if (data->data)
    x_free(data->data);
  x_free(data);
}

int s_service_check(const char *key)
{
  x_return_val_if_fail(key, -EINVAL);

  uint8_t size = strlen(key);
  uint8_t key_size = strlen(_g_service_key);

  return (size != key_size) ? 1 : strncmp(key, _g_service_key, size);
}
