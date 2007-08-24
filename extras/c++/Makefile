# krazy extras/c++ Makefile
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

# list of all extras to install
EXTRAS = \
multiclasses \
null \
style

# links to general purpose extras
EXTRALNS = \

#default installation location
PREFIX=/usr/local
DESTDIR=$(PREFIX)/libexec/krazy-extras/c++

all:

install::
	@if test ! -d $(DESTDIR); then \
		mkdir -p $(DESTDIR) || exit 1; \
	fi

	cp -f description.txt $(DESTDIR)

	list="$(EXTRAS)"; for extra in $$list; do \
		echo "Installing extra $$extra"; \
		cp -f $$extra $(DESTDIR)/$$extra; \
	done
	list="$(EXTRALNS)"; for extra in $$list; do \
		echo "Installing extra link $$extra"; \
		( cd $(DESTDIR) && rm -f $$extra && ln -s ../general/$$extra $$extra ); \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
