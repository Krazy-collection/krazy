# krazy docs Makefile
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

PERLDOC=perldoc

#default installation location
PREFIX=/usr/local

krazyrc.5 : krazyrc.pod
	$(PERLDOC) $? > $@

all: krazyrc.5

install:: all
	@if test ! -d $(DESTDIR); then \
		mkdir -p $(DESTDIR) || exit 1; \
	fi

	@if test ! -d $(DESTDIR)/man5; then \
		mkdir -p $(DESTDIR)/$(PREFIX)/share/man/man5 || exit 1; \
	fi

	list=krazyrc.5; for man in $$list; do \
		echo "Installing man page $$man"; \
		mv -f $$man $(DESTDIR)/$(PREFIX)/share/man/man5/$$man; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean

