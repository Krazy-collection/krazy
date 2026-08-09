# krazy kde4 sets Makefile
#
# SPDX-FileCopyrightText: 2010 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

# links to c++ plugins
CPP_PLUGINS = \
captruefalse \
constref \
copyright \
cpp \
crashy \
doublequote_chars \
doxytags \
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
fixuifiles \
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

# links to tips plugins
TIPS_PLUGINS = \
endswithnewline \
filenames \
insecurenet \
spelling \

# links to Messages.sh plugins
MSG_PLUGINS = \
rcappend \

# links to QML plugins
QML_PLUGINS = \
copyright \
endswithnewline \
filenames \
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
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-sets/kde4"

all:

install::
	@if test ! -d $(DESTDIR)$(INSTDIR); then \
		mkdir -p $(DESTDIR)$(INSTDIR) || exit 1; \
	fi

	cp -f description.txt $(DESTDIR)$(INSTDIR)

	list="$(CPP_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set c++ plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/c++/$$plugin $(DESTDIR)$(INSTDIR)/c++-$$plugin; \
	done
	list="$(UI_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set designer plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/designer/$$plugin $(DESTDIR)$(INSTDIR)/designer-$$plugin; \
	done
	list="$(DESKTOP_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set desktop plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/desktop/$$plugin $(DESTDIR)$(INSTDIR)/desktop-$$plugin; \
	done
	list="$(KCFG_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set kconfigxt plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/kconfigxt/$$plugin $(DESTDIR)$(INSTDIR)/kconfigxt-$$plugin; \
	done
	list="$(RC_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set kpartgui plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/kpartgui/$$plugin $(DESTDIR)$(INSTDIR)/kpartgui-$$plugin; \
	done
	list="$(TIPS_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set tips plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/tips/$$plugin $(DESTDIR)$(INSTDIR)/tips-$$plugin; \
	done
	list="$(MSG_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set messages plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/messages/$$plugin $(DESTDIR)$(INSTDIR)/messages-$$plugin; \
	done
	list="$(QML_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set qml plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/qml/$$plugin $(DESTDIR)$(INSTDIR)/qml-$$plugin; \
	done
	list="$(PERL_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set perl plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/perl/$$plugin $(DESTDIR)$(INSTDIR)/per-$$plugin; \
	done
	list="$(PYTHON_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set python plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/python/$$plugin $(DESTDIR)$(INSTDIR)/python-$$plugin; \
	done
	list="$(JSON_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set json plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/json/$$plugin $(DESTDIR)$(INSTDIR)/json-$$plugin; \
	done
	list="$(SVG_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set SVG plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/svg/$$plugin $(DESTDIR)$(INSTDIR)/svg-$$plugin; \
	done
	list="$(CMAKE_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde4 set cmake plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/cmake/$$plugin $(DESTDIR)$(INSTDIR)/cmake-$$plugin; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
