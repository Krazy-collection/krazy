TEMPLATE = app
TARGET = qsssyntax
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += qsssyntax.cpp
fix this QT += qml-private core-private

# install
target.path = $$[INSTALL_ROOT]/lib$(LIBSUFFIX)/krazy2/krazy-plugins/qss
sources.path = $$[INSTALL_ROOT]/lib$(LIBSUFFIX)/krazy2/krazy-plugins/qss/qsssyntax
INSTALLS += target sources
