# krazy c++ sets Makefile
#
# SPDX-FileCopyrightText: 2010 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

SET=c++

# links to c++ plugins
CPP_PLUGINS = \
constref \
cpp \
dpointer \
endswithnewline \
explicit \
filenames \
includes \
inline \
insecurenet \
operators \
postfixop \
spelling \
staticobjects \

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

clean:
	@echo "Nothing to clean"

realclean: clean
