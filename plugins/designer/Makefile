# krazy plugins/designer Makefile
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

# list of all plugins to install
PLUGINS = fixuifiles \

# links to general purpose plugins
PLUGINLNS = \
contractions \
endswithnewline \
i18ncheckarg \
spelling \

#default installation location
INSTDIR=$(PREFIX)/libexec/krazy-plugins/designer

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
