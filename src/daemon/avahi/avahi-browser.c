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

#include <avahi-client/lookup.h>
#include <libdaemon/dlog.h>
#include "avahi-browser.h"
#include "daemon-alloc.h"
#include "daemon-cond.h"

struct s_browser_data *s_browser_data_new(const char *address,
  const char *domain, const char *name, uint16_t port, const char *txt,
  const char *type)
{
  struct s_browser_data *data = daemon_malloc(sizeof(struct s_browser_data));
  data->address = address ? strdup(address) : NULL;
  data->domain = domain ? strdup(domain) : NULL;
  data->name = name ? strdup(name) : NULL;
  data->port = port;
  data->txt = txt ? strdup(txt) : NULL;
  data->type = type ? strdup(type) : NULL;
  return data;
}

void s_browser_data_free(struct s_browser_data *data)
{
  daemon_return_if_fail(data);

  if (data->address)
    daemon_free(data->address);
  if (data->domain)
    daemon_free(data->domain);
  if (data->name)
    daemon_free(data->name);
  if (data->txt)
    daemon_free(data->txt);
  if (data->type)
    daemon_free(data->type);
  daemon_free(data);
}

struct s_browser {
  AvahiServiceBrowser *browser;
  struct s_service_data *data;
  struct s_browser_funcs funcs;
  void *userdata;
};

/* codecheck_ignore[COMPLEX_MACRO] */
#define _s_browser_min(str1, str2) \
  (strlen(str1) < strlen(str2)) ? strlen(str1) : strlen(str2)

static void _s_browser_resolver_cbk(AvahiServiceResolver *resolver,
  daemon_unused AvahiIfIndex interface, daemon_unused AvahiProtocol protocol,
  AvahiResolverEvent event, const char *name, const char *type,
  const char *domain, daemon_unused const char *host_name,
  const AvahiAddress *address, uint16_t port, AvahiStringList *txt,
  daemon_unused AvahiLookupResultFlags flags, struct s_browser *browser)
{
  daemon_return_if_fail(browser);

  AvahiClient *client = avahi_service_resolver_get_client(resolver);
  /* Called whenever a service has been resolved successfully or timed out */
  switch (event) {
  case AVAHI_RESOLVER_FAILURE:
    browser->funcs.failure(browser->userdata, avahi_client_errno(client));
    break;
  case AVAHI_RESOLVER_FOUND:
    if (strncmp(name, "cerebellum", _s_browser_min(name, "cerebellum")) == 0) {
      char addr_str[AVAHI_ADDRESS_STR_MAX] = { 0, };
      avahi_address_snprint(addr_str, sizeof(*address), address);
      char *txt_str = avahi_string_list_to_string(txt);
      browser->funcs.find(browser->userdata, s_browser_data_new(addr_str,
        domain, name, port, txt_str, type));
      daemon_free(txt_str);
    }
    break;
  }
  avahi_service_resolver_free(resolver);
}

static void _s_browser_cbk(AvahiServiceBrowser *avahi_browser,
  AvahiIfIndex interface, AvahiProtocol protocol, AvahiBrowserEvent event,
  const char *name, const char *type, const char *domain,
  daemon_unused AvahiLookupResultFlags flags, struct s_browser *browser)
{
  daemon_return_if_fail(browser);

  AvahiClient *client = avahi_service_browser_get_client(avahi_browser);
  switch (event) {
  case AVAHI_BROWSER_FAILURE:
    browser->funcs.failure(browser->userdata, avahi_client_errno(client));
    break;
  case AVAHI_BROWSER_NEW:
    if (!avahi_service_resolver_new(client, interface, protocol, name, type,
          domain, AVAHI_PROTO_UNSPEC, 0,
          (AvahiServiceResolverCallback)_s_browser_resolver_cbk, browser))
        browser->funcs.failure(browser->userdata, avahi_client_errno(client));
    break;
  case AVAHI_BROWSER_REMOVE: {
    struct s_browser_data data = {
      .address = NULL,
      .domain = (char *)domain,
      .name = (char *)name,
      .port = 0,
      .txt = NULL,
      .type = (char *)type
    };
    browser->funcs.remove(browser->userdata, &data);
    break;
  }
  case AVAHI_BROWSER_ALL_FOR_NOW:
  case AVAHI_BROWSER_CACHE_EXHAUSTED:
    break;
  }
}

struct s_browser *s_browser_new(struct s_client *client,
  struct s_service_data *data, const struct s_browser_funcs *funcs,
  void *userdata)
{
  daemon_return_val_if_fail(client, NULL);
  daemon_return_val_if_fail(data, NULL);
  daemon_return_val_if_fail(funcs, NULL);

  struct s_browser *browser = daemon_malloc(sizeof(struct s_browser));
  AvahiClient *avahi_client = s_client_toavahi(client);
  browser->data = data;
  browser->funcs = *funcs;
  browser->userdata = userdata;
  browser->browser = avahi_service_browser_new(avahi_client, data->interface,
    data->protocol, data->type, NULL, 0,
    (AvahiServiceBrowserCallback)_s_browser_cbk, browser);

  if (!browser->browser)
    goto error;

  return browser;

error:
  daemon_log(LOG_ERR, "failed to allocate a browser instance\n");
  s_browser_free(browser);
  return NULL;
}

void s_browser_free(struct s_browser *browser)
{
  daemon_return_if_fail(browser);

  avahi_service_browser_free(browser->browser);
  s_service_free(browser->data);
  daemon_free(browser);
}
