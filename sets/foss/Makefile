# krazy FOSS sets Makefile
#
# SPDX-FileCopyrightText: 2010 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

# links to c++ plugins
CPP_PLUGINS = \
copyright \
insecurenet \
license \
spelling \

# links to QML plugins
QML_PLUGINS = \
copyright \
insecurenet \
license \
spelling \

# links to Perl plugins
PERL_PLUGINS = \
copyright \
endswithnewline \
filenames \
insecurenet \
license \
spelling \

# links to Python plugins
PYTHON_PLUGINS = \
copyright \
endswithnewline \
filenames \
insecurenet \
license \
spelling \

# links to JSON plugins
JSON_PLUGINS = \
endswithnewline \
filenames \
insecurenet \
spelling \
validate \

# links to SVG plugins
SVG_PLUGINS = \
filenames \
insecurenet \
spelling \
validate \

#default installation location
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-sets/foss"

all:

install::
	@if test ! -d $(DESTDIR)$(INSTDIR); then \
		mkdir -p $(DESTDIR)$(INSTDIR) || exit 1; \
	fi

	cp -f description.txt $(DESTDIR)$(INSTDIR)

	list="$(CPP_PLUGINS)"; for plugin in $$list; do \
		echo "Installing foss set c++ plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/c++/$$plugin $(DESTDIR)$(INSTDIR)/c++-$$plugin; \
	done

	list="$(QML_PLUGINS)"; for plugin in $$list; do \
		echo "Installing foss set qml plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/qml/$$plugin $(DESTDIR)$(INSTDIR)/qml-$$plugin; \
	done

	list="$(PERL_PLUGINS)"; for plugin in $$list; do \
		echo "Installing foss set perl plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/perl/$$plugin $(DESTDIR)$(INSTDIR)/perl-$$plugin; \
	done

	list="$(PYTHON_PLUGINS)"; for plugin in $$list; do \
		echo "Installing foss set python plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/python/$$plugin $(DESTDIR)$(INSTDIR)/python-$$plugin; \
	done

	list="$(JSON_PLUGINS)"; for plugin in $$list; do \
		echo "Installing foss set json plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/json/$$plugin $(DESTDIR)$(INSTDIR)/json-$$plugin; \
	done

	list="$(SVG_PLUGINS)"; for plugin in $$list; do \
		echo "Installing foss set SVG plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/svg/$$plugin $(DESTDIR)$(INSTDIR)/svg-$$plugin; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
