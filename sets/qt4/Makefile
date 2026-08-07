# krazy qt4 sets Makefile
#
# SPDX-FileCopyrightText: 2010 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

SET=qt4

# links to c++ plugins
CPP_PLUGINS = \
captruefalse \
constref \
cpp \
doublequote_chars \
dpointer \
emptystrcompare \
endswithnewline \
explicit \
filenames \
includes \
inline \
insecurenet \
normalize \
nullstrassign \
nullstrcompare \
operators \
postfixop \
qminmax \
qobject \
sigsandslots \
spelling \
staticobjects \
strings \
syscalls \
typedefs \

# links to designer plugins
UI_PLUGINS = \
endswithnewline \
filenames \
insecurenet \
spelling \

# links to desktop plugins
DESKTOP_PLUGINS = \
endswithnewline \
filenames \
insecurenet \
spelling \
validate \

# links to qdoc plugins
QDOC_PLUGINS = \
endswithnewline \
filenames \
insecurenet \
spelling \

# links to QML plugins
QML_PLUGINS = \
endswithnewline \
filenames \
insecurenet \
spelling \

# links to Perl plugins
PERL_PLUGINS = \
endswithnewline \
filenames \
insecurenet \
spelling \

# links to Python plugins
PYTHON_PLUGINS = \
endswithnewline \
filenames \
insecurenet \
qtemits \
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
validate \

# links to CMake plugins
CMAKE_PLUGINS = \
endswithnewline \
filenames \
insecurenet \
spelling \

#default installation location
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-sets/${SET}"

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
	list="$(UI_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set designer plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/designer/$$plugin $(DESTDIR)$(INSTDIR)/designer-$$plugin; \
	done
	list="$(DESKTOP_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set desktop plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/desktop/$$plugin $(DESTDIR)$(INSTDIR)/desktop-$$plugin; \
	done
	list="$(QDOC_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set qdoc plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/qdoc/$$plugin $(DESTDIR)$(INSTDIR)/qdoc-$$plugin; \
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
		echo "Installing ${SET} set SVG plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/svg/$$plugin $(DESTDIR)$(INSTDIR)/svg-$$plugin; \
	done
	list="$(CMAKE_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set cmake plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/cmake/$$plugin $(DESTDIR)$(INSTDIR)/cmake-$$plugin; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
