# krazy non-binary plugins Makefile
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

# list of all plugins to install
PLUGINS = "\
"

# links to general purpose plugins
PLUGINLNS = "\
contractions \
endswithnewline \
i18ncheckarg \
spelling \
"

#default installation location
PREFIX=/usr/local
INSTALLSCRIPT=$(PREFIX)/libexec/krazy-plugins/po
PERM=775

all:

install::
	@if test ! -d $(INSTALLSCRIPT); then \
		mkdir -p $(INSTALLSCRIPT) || exit 1; \
	fi
	chmod $(PERM) $(INSTALLSCRIPT)

	cp -f description.txt $(INSTALLSCRIPT)
	chmod 664 $(INSTALLSCRIPT)/description.txt

	list=$(PLUGINS); for plugin in $$list; do \
		echo "Installing plugin $$plugin"; \
		cp -f $$plugin $(INSTALLSCRIPT)/$$plugin; \
		chmod $(PERM) $(INSTALLSCRIPT)/$$plugin; \
	done
	list=$(PLUGINLNS); for plugin in $$list; do \
		echo "Installing plugin link $$plugin"; \
		( cd $(INSTALLSCRIPT) && rm -f $$plugin && ln -s ../general/$$plugin $$plugin ); \
	done

# remove old plugins that are no longer used
install::
	rm -f $(INSTALLSCRIPT)/doxytags.0

clean:
	@echo "Nothing to clean"

realclean: clean
