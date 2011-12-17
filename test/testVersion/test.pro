QT += testlib
CONFIG += console debug_and_release
TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += testVersion.cpp \
           ../../core/version.cpp
