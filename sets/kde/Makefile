# krazy kde sets Makefile
#
# SPDX-FileCopyrightText: 2025 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

SET=kde

# links to c++ plugins
CPP_PLUGINS = \
constref \
copyright \
cpp \
crashy \
doublequote_chars \
dpointer \
emptystrcompare \
endswithnewline \
explicit \
filenames \
i18ncheckarg \
includes \
inline \
insecurenet \
license \
normalize \
nullstrassign \
nullstrcompare \
operators \
postfixop \
qbytearray \
qclasses \
qmethods \
qobject \
reuse \
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
i18ncheckarg \
insecurenet \
qclasses \
spelling \

# links to desktop plugins
DESKTOP_PLUGINS = \
endswithnewline \
filenames \
insecurenet \
spelling \
validate \

# lines to kconfigxt plugins
KCFG_PLUGINS = \
endswithnewline \
filenames \
i18ncheckarg \
insecurenet \
spelling \
validate \

# links to kpartgui plugins
RC_PLUGINS = \
endswithnewline \
filenames \
i18ncheckarg \
insecurenet \
spelling \
validate \

# links to QML plugins
QML_PLUGINS = \
copyright \
endswithnewline \
filenames \
insecurenet \
license \
reuse \
spelling \

# links to Perl plugins
PERL_PLUGINS = \
copyright \
endswithnewline \
filenames \
insecurenet \
license \
reuse \
spelling \

# links to Python plugins
PYTHON_PLUGINS = \
copyright \
endswithnewline \
filenames \
insecurenet \
license \
qtemits \
reuse \
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
copyright \
endswithnewline \
filenames \
license \
insecurenet \
spelling \
reuse \

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
	list="$(KCFG_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set kconfigxt plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/kconfigxt/$$plugin $(DESTDIR)$(INSTDIR)/kconfigxt-$$plugin; \
	done
	list="$(RC_PLUGINS)"; for plugin in $$list; do \
		echo "Installing ${SET} set kpartgui plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/kpartgui/$$plugin $(DESTDIR)$(INSTDIR)/kpartgui-$$plugin; \
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
