# krazy helpers Makefile
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

# list of all helpers to install
HELPERS = \
getInstalledHeaders.pl

#default installation location
PREFIX=/usr/local
INSTDIR=$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-helpers

all:

install::
	@if test ! -d $(DESTDIR)$(INSTDIR); then \
		mkdir -p $(DESTDIR)$(INSTDIR) || exit 1; \
	fi
	list="$(HELPERS)"; for helper in $$list; do \
		echo "Installing helper $$helper"; \
		cp -f $$helper $(DESTDIR)$(INSTDIR)/$$helper; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean

