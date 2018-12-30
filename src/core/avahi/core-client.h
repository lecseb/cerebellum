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

#ifndef _CORE_AVAHI_CORE_CLIENT_H_
# define _CORE_AVAHI_CORE_CLIENT_H_

# include "avahi/avahi-client.h"

/**
 * @brief Get the avahi client interface
 * @param [in] client: client structure to browse
 * @return a valid pointer on success, NULL on error
 */
AvahiClient *s_client_toavahi(struct s_client *client);

#endif /* !_CORE_AVAHI_CORE_CLIENT_H_ */
