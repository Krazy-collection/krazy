# krazy extras/cmake Makefile
#
# SPDX-FileCopyrightText: 2012 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

# list of all extras to install
EXTRAS = \
style \

# links to general purpose extras
EXTRALNS = \
copyright-reuse \
license-reuse \

#default installation location
PREFIX=/usr/local
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-extras/cmake"

all:

install::
	@if test ! -d $(DESTDIR)$(INSTDIR); then \
		mkdir -p $(DESTDIR)$(INSTDIR) || exit 1; \
	fi

	cp -f description.txt $(DESTDIR)$(INSTDIR)

	list="$(EXTRAS)"; for extra in $$list; do \
		echo "Installing extra $$extra"; \
		cp -f $$extra $(DESTDIR)$(INSTDIR)/$$extra; \
	done
	list="$(EXTRALNS)"; for extra in $$list; do \
		echo "Installing extra link $$extra"; \
		ln -sf ../general/$$extra $(DESTDIR)$(INSTDIR)/$$extra; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
