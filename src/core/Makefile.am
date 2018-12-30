# This file is part of mytank-module.

# mytank-module is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# mytank-module is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with mytank-module.  If not, see <https://www.gnu.org/licenses/>.

include $(top_builddir)/script/check.mk

noinst_LTLIBRARIES= libcore.la

libcore_la_CFLAGS= \
	$(AM_CFLAGS) \
	$(avahi_client_CFLAGS) \
	$(dbus_CFLAGS) \
	$(libcrypto_CFLAGS) \
	$(libevent_CFLAGS) \
	$(libevent_openssl_CFLAGS) \
	$(libssl_CFLAGS) \
	-I$(top_srcdir)/src/core/include \
	-fvisibility=hidden

noinst_HEADERS= \
	avahi/core-client.h \
	loop/loop-idle.h \
	loop/avahi/avahi-timer.h \
	loop/avahi/avahi-watch.h \
	include/alloc.h \
	include/cond.h \
	include/export.h \
	include/log.h \
	include/avahi/avahi-browser.h \
	include/avahi/avahi-client.h \
	include/avahi/avahi-service.h \
	include/loop/loop.h \
	include/ssl/ssl.h \
	include/ssl/ssl-client.h \
	include/ssl/ssl-packet.h

libcore_la_SOURCES= \
	avahi/avahi-browser.c \
	avahi/avahi-client.c \
	avahi/avahi-service.c \
	loop/loop.c \
	loop/loop-idle.c \
	loop/avahi/avahi-loop.c \
	loop/avahi/avahi-timer.c \
	loop/avahi/avahi-watch.c \
	ssl/ssl-client.c

libcore_la_LIBADD= \
	$(avahi_client_LIBS) \
	$(dbus_LIBS) \
	$(libcrypto_LIBS) \
	$(libevent_LIBS) \
	$(libevent_openssl_LIBS) \
	$(libssl_LIBS)

# eval to create the coding style rule
$(eval $(call check, $(sort $(noinst_HEADERS) $(libcore_la_SOURCES))))
