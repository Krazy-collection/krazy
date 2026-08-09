# krazy plugins/qml Makefile
#
# SPDX-FileCopyrightText: 2010 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

# list of all plugins to install
PLUGINS = \

# links to general purpose plugins
PLUGINLNS = \
copyright \
endswithnewline \
filenames \
license \
insecurenet \
spelling \

#default installation location
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-plugins/qml"

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
	list="$(PLUGINLNS)"; for plugin in $$list; do \
		echo "Installing plugin link $$plugin"; \
		ln -sf ../general/$$plugin $(DESTDIR)$(INSTDIR)/$$plugin; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
