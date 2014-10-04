TEMPLATE = app
TARGET = qmlsyntax
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += qmlsyntax.cpp
QT += qml-private core-private

# install
target.path = $$[INSTALL_ROOT]/lib$(LIBSUFFIX)/krazy2/krazy-plugins/qml
sources.path = $$[INSTALL_ROOT]/lib$(LIBSUFFIX)/krazy2/krazy-plugins/qml/qmlsyntax
INSTALLS += target sources
