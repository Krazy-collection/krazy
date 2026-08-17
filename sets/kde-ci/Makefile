# krazy kde-ci sets Makefile
#
# SPDX-FileCopyrightText: 2026 Allen Winter <winter@kde.org>
# SPDX-License-Identifier: GPL-2.0-or-later
#

SET="kde-ci"

# links to c++ checker plugins
CPP_PLUGINS = \
copyright \
cpp \
crashy \
i18ncheckarg \
includes \
insecurenet \
license \
qclasses \
qmethods \
syscalls \
typedefs \

# links to designer checker plugins
UI_PLUGINS = \
i18ncheckarg \
insecurenet \
qclasses \

# links to desktop checker plugins
DESKTOP_PLUGINS = \
insecurenet \
validate \

# lines to kconfigxt checker plugins
KCFG_PLUGINS = \
i18ncheckarg \
insecurenet \
validate \

# links to kpartgui checker plugins
RC_PLUGINS = \
i18ncheckarg \
insecurenet \
validate \

# links to QML checker plugins
QML_PLUGINS = \
copyright \
insecurenet \
license \

# links to Perl checker plugins
PERL_PLUGINS = \
copyright \
license \
insecurenet \

# links to Python checker plugins
PYTHON_PLUGINS = \
copyright \
license \
insecurenet \

# links to JSON checker plugins
JSON_PLUGINS = \
insecurenet \
validate \

# links to SVG checker plugins
SVG_PLUGINS = \
validate \

# links to CMake checker plugins
CMAKE_PLUGINS = \
copyright \
license \
insecurenet \

#default installation location
INSTDIR="$(PREFIX)/lib$(LIBSUFFIX)/krazy2/krazy-sets/$(SET)"

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
