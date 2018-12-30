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

SUBDIRS=

bin_PROGRAMS= cerebellum_daemon

cerebellum_daemon_CFLAGS= \
	$(AM_CFLAGS) \
	$(dbus_CFLAGS) \
	-I$(top_srcdir)/src/core/include

noinst_HEADERS= \
	cerebellum.h

cerebellum_daemon_SOURCES= \
	cerebellum.c \
	cerebellum-browser.c \
	cerebellum-client.c \
	cerebellum-main.c \
	cerebellum-ssl.c

cerebellum_daemon_LDFLAGS= \
	$(dbus_LIBS) \
	$(top_srcdir)/src/core/libcore.la

# eval to create the coding style rule
$(eval $(call check, $(sort $(noinst_HEADERS) $(cerebellum_daemon_SOURCES))))
