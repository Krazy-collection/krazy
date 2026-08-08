# krazy docs Makefile
#
# SPDX-FileCopyrightText: 2007 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

PERLDOC=perldoc

#default installation location
PREFIX=/usr/local

krazyrc.5 : krazyrc.pod
	$(PERLDOC) $? > $@

all: krazyrc.5

install:: all
	@if test ! -d $(DESTDIR); then \
		mkdir -p $(DESTDIR) || exit 1; \
	fi

	@if test ! -d $(DESTDIR)/man5; then \
		mkdir -p $(DESTDIR)/$(PREFIX)/man/man5 || exit 1; \
	fi

	list=krazyrc.5; for man in $$list; do \
		echo "Installing man page $$man"; \
		mv -f $$man $(DESTDIR)/$(PREFIX)/man/man5/$$man; \
	done

clean:
	@echo "Nothing to clean"

realclean: clean
