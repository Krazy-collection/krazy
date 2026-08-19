# krazy sets Makefile
#
# SPDX-FileCopyrightText: 2010 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

# list of all set subdirs to install
SUBDIRS = \
c++ \
qt \
kde \
kde-ci \
foss \

#default installation location
PREFIX=/usr/local
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy/krazy-sets"

all:

install::
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Installing set subdir $$dir"; \
		( cd $$dir && $(MAKE) install ); \
	done

clean:
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Cleaning set subdir $$dir"; \
		( cd $$dir && $(MAKE) clean ); \
	done

realclean: clean
