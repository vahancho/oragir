message("You are running qmake on oragir.pro file.")

TEMPLATE = app
TARGET = oragir
DESTDIR = ./release
QT += network xml sql

CONFIG += release
win32:CONFIG += embed_manifest_exe

INCLUDEPATH += ./GeneratedFiles \
               ./GeneratedFiles/release \
			   $(ProjectDir)/. \
               .
win32:LIBS += -lqtmain -lQtCore4 -lQtGui4 -lQtNetwork4 -lQtSql
unix:LIBS += -lQtCore -lQtGui -lQtNetwork -lQtSql

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

# Adds icon to the executable file on Windows.
win32:RC_FILE = application.rc

#Include file(s)
include(oragir.pri)
