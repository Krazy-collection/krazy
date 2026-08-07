# krazy plugins Makefile
#
# SPDX-FileCopyrightText: 2007 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

# list of all plugin subdirs to install
SUBDIRS = \
general \
c++ \
cmake \
designer \
desktop \
kconfigxt \
messages \
kpartgui \
tips \
qml \
qdoc \
perl \
python \
json \
svg

#default installation location
PREFIX=/usr/local
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy/krazy-plugins"

all:

install::
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Installing plugin subdir $$dir"; \
		( cd $$dir && $(MAKE) install ); \
	done

clean:
	list="$(SUBDIRS)"; for dir in $$list; do \
		echo "Cleaning plugin subdir $$dir"; \
		( cd $$dir && $(MAKE) clean ); \
	done

realclean: clean
