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

AC_INIT([cerebellum], [0.1])
AM_INIT_AUTOMAKE([subdir-objects foreign])
AM_CONFIG_HEADER([config.h])

# Check programs
AC_PROG_CC
AM_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LIBTOOL(libtool)

# Checks header files
AC_HEADER_STDC

# Check library
PKG_CHECK_MODULES([avahi_client], [avahi-client])
PKG_CHECK_MODULES([dbus], [dbus-1])
PKG_CHECK_MODULES([libcrypto], [libcrypto])
PKG_CHECK_MODULES([libevent], [libevent])
PKG_CHECK_MODULES([libevent_openssl], [libevent_openssl])
PKG_CHECK_MODULES([libssl], [libssl])

my_CFLAGS="\
-W \
-Werror \
-Wall \
-Wchar-subscripts \
-Wformat-security \
-Wmissing-declarations \
-Wmissing-prototypes \
-Wnested-externs \
-Wpointer-arith \
-Wshadow \
-Wsign-compare \
-Wstrict-prototypes \
-Wtype-limits \
-I`pwd`/src \
"

# debug mode activation
AC_ARG_ENABLE(debug, AS_HELP_STRING([--enable-debug]),
	[extra_CFLAGS="-g -ggdb"], [extra_CFLAGS="-O3"])

AC_SUBST([AM_CFLAGS], ["$AM_CFLAGS $my_CFLAGS $extra_CFLAGS"])

# Output generated file
AC_CONFIG_FILES([
	Makefile
	src/Makefile
	src/application/Makefile
	src/core/Makefile
	src/daemon/Makefile
])

AC_OUTPUT

AC_MSG_RESULT([
    $PACKAGE $VERSION
    ==========================

    prefix:              ${prefix}
    sysconfdir:          ${sysconfdir}
    libdir:              ${libdir}
    rootlibdir:          ${rootlibdir}
    includedir:          ${includedir}
    bindir:              ${bindir}

    C compiler:          ${CC}
    CFLAGS:              ${AM_CFLAGS}
    LDFLAGS:             ${AM_LDFLAGS}
])
