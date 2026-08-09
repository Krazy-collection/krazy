# krazy extras Makefile
#
# SPDX-FileCopyrightText: 2007 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later

# list of all extra subdirs to install
SUBDIRS = \
c++ \
cmake \
designer \
desktop \
general \
kconfigxt \
kpartgui \
perl \
python \
json \

#default installation location
PREFIX=/usr/local
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-extras"

all:

install::
	@if test ! -d $(DESTDIR)$(INSTDIR); then \
		mkdir -p $(DESTDIR)$(INSTDIR) || exit 1; \
	fi
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Installing extras subdir $$dir"; \
		( cd $$dir && $(MAKE) install ); \
	done

clean:
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Cleaning extras subdir $$dir"; \
		( cd $$dir && $(MAKE) clean ); \
	done

realclean: clean
