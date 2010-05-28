# krazy sets Makefile
#
# Copyright 2010 by Allen Winter <winter@kde.org>
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

# list of all set subdirs to install
SUBDIRS = \
c++ \
qt4 \
kde4 \
foss \

#default installation location
PREFIX=/usr/local
INSTDIR=$(PREFIX)/lib$(LIBSUFFIX)/krazy/krazy-sets

all:

install::
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Installing set subdir $$dir"; \
		( cd $$dir && make install ); \
	done

clean:
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Cleaning set subdir $$dir"; \
		( cd $$dir && make clean ); \
	done

realclean: clean
