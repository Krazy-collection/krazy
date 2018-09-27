# krazy FOSS sets Makefile
#
# Copyright 2010-2018 by Allen Winter <winter@kde.org>
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

SET=foss

# links to c++ plugins
CPP_PLUGINS = \
copyright \
license \

# links to QML plugins
QML_PLUGINS = \
copyright \
license \

# links to Perl plugins
PERL_PLUGINS = \
copyright \
endswithnewline \
filenames \
license \

# links to Python plugins
PYTHON_PLUGINS = \
copyright \
endswithnewline \
filenames \
license \

# links to JSON plugins
JSON_PLUGINS = \
endswithnewline \
filenames \
validate \

# links to SVG plugins
SVG_PLUGINS = \
filenames \
validate \

# links to QSS plugins
QSS_PLUGINS = \
endswithnewline \
filenames \
validate \

#default installation location
INSTDIR=$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-sets/${SET}

all:

install::
	@if test ! -d $(DESTDIR)$(INSTDIR); then \
		mkdir -p $(DESTDIR)$(INSTDIR) || exit 1; \
	fi

	cp -f description.txt $(DESTDIR)$(INSTDIR)

	list="$(CPP_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set c++ plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/c++/$$plugin $(DESTDIR)$(INSTDIR)/c++-$$plugin; \
	done

	list="$(QML_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set qml plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/qml/$$plugin $(DESTDIR)$(INSTDIR)/qml-$$plugin; \
	done

	list="$(PERL_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set perl plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/perl/$$plugin $(DESTDIR)$(INSTDIR)/perl-$$plugin; \
	done

	list="$(PYTHON_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set python plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/python/$$plugin $(DESTDIR)$(INSTDIR)/python-$$plugin; \
	done

	list="$(JSON_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set json plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/json/$$plugin $(DESTDIR)$(INSTDIR)/json-$$plugin; \
	done

	list="$(SVG_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set svg plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/svg/$$plugin $(DESTDIR)$(INSTDIR)/svg-$$plugin; \
	done

	list="$(QSS_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set qss plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/qss/$$plugin $(DESTDIR)$(INSTDIR)/qss-$$plugin; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
