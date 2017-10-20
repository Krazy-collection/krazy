# krazy plugins Makefile
#
# Copyright 2007-2008,2010,2012-2013,2015 by Allen Winter <winter@kde.org>
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
SUBDIRS = \
general \
c++ \
cmake \
designer \
desktop \
kconfigxt \
messages \
kpartgui \
tips \
qml \
qdoc \
perl \
python \
json \
svg

#default installation location
PREFIX=/usr/local
INSTDIR=$(PREFIX)/lib$(LIBSUFFIX)/krazy/krazy-plugins

all:

install::
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Installing plugin subdir $$dir"; \
		( cd $$dir && $(MAKE) install ); \
	done

clean:
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Cleaning plugin subdir $$dir"; \
		( cd $$dir && $(MAKE) clean ); \
	done

realclean: clean
