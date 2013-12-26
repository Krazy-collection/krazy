# krazy kde5 sets Makefile
#
# Copyright 2013 by Allen Winter <winter@kde.org>
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

# links to c++ plugins
CPP_PLUGINS = \
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
foreach \
i18ncheckarg \
includes \
inline \
license \
normalize \
nullstrassign \
nullstrcompare \
operators \
passbyvalue \
postfixop \
qbytearray \
qclasses \
qmethods \
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
fixuifiles \
i18ncheckarg \
qclasses \
spelling \

# links to desktop plugins
DESKTOP_PLUGINS = \
endswithnewline \
iconnames \
spelling \
validate \

# lines to kconfigxt plugins
KCFG_PLUGINS = \
endswithnewline \
i18ncheckarg \
spelling \
validate \

# links to kpartgui plugins
RC_PLUGINS = \
endswithnewline \
i18ncheckarg \
spelling \
validate \

# links to tips plugins
TIPS_PLUGINS = \
endswithnewline \
spelling \

# links to Messages.sh plugins
MSG_PLUGINS = \
rcappend \

# links to QML plugins
QML_PLUGINS = \
copyright \
endswithnewline \
iconnames \
license \
spelling \

# links to Python plugins
PYTHON_PLUGINS = \
copyright \
endswithnewline \
license \
spelling \

# links to CMake plugins
CMAKE_PLUGINS = \
endswithnewline \
spelling \

#default installation location
INSTDIR=$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-sets/kde5

all:

install::
	@if test ! -d $(DESTDIR)$(INSTDIR); then \
		mkdir -p $(DESTDIR)$(INSTDIR) || exit 1; \
	fi

	cp -f description.txt $(DESTDIR)$(INSTDIR)

	list="$(CPP_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde5 set c++ plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/c++/$$plugin $(DESTDIR)$(INSTDIR)/c++-$$plugin; \
	done
	list="$(UI_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde5 set designer plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/designer/$$plugin $(DESTDIR)$(INSTDIR)/designer-$$plugin; \
	done
	list="$(DESKTOP_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde5 set desktop plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/desktop/$$plugin $(DESTDIR)$(INSTDIR)/desktop-$$plugin; \
	done
	list="$(KCFG_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde5 set kconfigxt plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/kconfigxt/$$plugin $(DESTDIR)$(INSTDIR)/kconfigxt-$$plugin; \
	done
	list="$(RC_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde5 set kpartgui plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/kpartgui/$$plugin $(DESTDIR)$(INSTDIR)/kpartgui-$$plugin; \
	done
	list="$(TIPS_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde5 set tips plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/tips/$$plugin $(DESTDIR)$(INSTDIR)/tips-$$plugin; \
	done
	list="$(MSG_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde5 set messages plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/messages/$$plugin $(DESTDIR)$(INSTDIR)/messages-$$plugin; \
	done
	list="$(QML_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde5 set qml plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/qml/$$plugin $(DESTDIR)$(INSTDIR)/qml-$$plugin; \
	done
	list="$(PYTHON_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde5 set python plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/python/$$plugin $(DESTDIR)$(INSTDIR)/python-$$plugin; \
	done
	list="$(CMAKE_PLUGINS)"; for plugin in $$list; do \
		echo "Installing kde5 set cmake plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/cmake/$$plugin $(DESTDIR)$(INSTDIR)/cmake-$$plugin; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
