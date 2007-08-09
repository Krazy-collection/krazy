# krazy helpers Makefile
#
# Copyright (C) 2007 by Allen Winter <winter@kde.org>
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

# list of all plugin subdirs to install
SUBDIRS = "\
general \
c++ \
designer \
desktop \
"

#default installation location
PREFIX=/usr/local
INSTALLSCRIPT=$(PREFIX)/libexec/krazy-plugins
PERM=775

all:

install::
	@if test ! -d $(INSTALLSCRIPT); then \
		mkdir -p $(INSTALLSCRIPT) || exit 1; \
		chmod $(PERM) $(INSTALLSCRIPT); \
	fi
	chmod $(PERM) $(INSTALLSCRIPT)
	list=$(SUBDIRS); for dir in $$list; do \
		echo "Installing plugin subdir $$dir"; \
		( cd $$dir && make install ); \
	done

# remove old subdirs that are no longer used
#install::
#	rm -rf $(INSTALLSCRIPT)/fred

clean:
	list=$(SUBDIRS); for dir in $$list; do \
		echo "Cleaning plugin subdir $$dir"; \
		( cd $$dir && make clean ); \
	done

realclean: clean
