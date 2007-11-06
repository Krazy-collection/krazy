# krazy extras Makefile
#
# Copyright 2007 by Allen Winter <winter@kde.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See th
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

# list of all extra subdirs to install
SUBDIRS = \
c++

#default installation location
PREFIX=/usr/local
INSTDIR=$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-extras

all:

install::
	@if test ! -d $(DESTDIR)$(INSTDIR); then \
		mkdir -p $(DESTDIR)$(INSTDIR) || exit 1; \
	fi
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Installing extras subdir $$dir"; \
		( cd $$dir && make install ); \
	done

clean:
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Cleaning extras subdir $$dir"; \
		( cd $$dir && make clean ); \
	done

realclean: clean
