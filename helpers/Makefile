# krazy helpers Makefile
#
# SPDX-FileCopyrightText: 2007 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

# list of all helpers to install
HELPERS = \
krazyfindfiles.pl \
krazyprojecttype.pl \

#default installation location
PREFIX=/usr/local
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-helpers"

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
