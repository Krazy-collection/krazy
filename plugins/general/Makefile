# krazy plugins/general Makefile
#
# SPDX-FileCopyrightText: 2007 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

# list of all plugins to install
PLUGINS = \
copyright \
endswithnewline \
filenames \
i18ncheckarg \
license \
insecurenet \
qclasses \
spelling \

#default installation location
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-plugins/general"

all:

install::
	@if test ! -d $(DESTDIR)$(INSTDIR); then \
		mkdir -p $(DESTDIR)$(INSTDIR) || exit 1; \
	fi

	cp -f description.txt $(DESTDIR)$(INSTDIR)

	list="$(PLUGINS)"; for plugin in $$list; do \
		echo "Installing plugin $$plugin"; \
		cp -f $$plugin $(DESTDIR)$(INSTDIR)/$$plugin; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
