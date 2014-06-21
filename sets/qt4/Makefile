# krazy qt4 sets Makefile
#
# Copyright 2010-2011,2014 by Allen Winter <winter@kde.org>
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
captruefalse \
constref \
cpp \
doublequote_chars \
emptystrcompare \
endswithnewline \
explicit \
foreach \
includes \
normalize \
nullstrassign \
nullstrcompare \
operators \
postfixop \
qminmax \
qobject \
spelling \
staticobjects \
typedefs \

# links to qml plugins
QML_PLUGINS = \
endswithnewline \
spelling \

# links to designer plugins
UI_PLUGINS = \
endswithnewline \
spelling \

# links to qdoc plugins
QDOC_PLUGINS = \
endswithnewline \
spelling \

#default installation location
INSTDIR=$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-sets/qt4

all:

install::
	@if test ! -d $(DESTDIR)$(INSTDIR); then \
		mkdir -p $(DESTDIR)$(INSTDIR) || exit 1; \
	fi

	cp -f description.txt $(DESTDIR)$(INSTDIR)

	list="$(CPP_PLUGINS)"; for plugin in $$list; do \
		echo "Installing qt4 set c++ plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/c++/$$plugin $(DESTDIR)$(INSTDIR)/c++-$$plugin; \
	done
	list="$(QML_PLUGINS)"; for plugin in $$list; do \
		echo "Installing qt4 set qml plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/qml/$$plugin $(DESTDIR)$(INSTDIR)/qml-$$plugin; \
	done
	list="$(UI_PLUGINS)"; for plugin in $$list; do \
		echo "Installing qt4 set designer plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/designer/$$plugin $(DESTDIR)$(INSTDIR)/designer-$$plugin; \
	done
	list="$(QDOC_PLUGINS)"; for plugin in $$list; do \
		echo "Installing qt4 set qdoc plugin link $$plugin"; \
		ln -sf ../../krazy-plugins/qdoc/$$plugin $(DESTDIR)$(INSTDIR)/qdoc-$$plugin; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
