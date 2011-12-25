message("You are running qmake on oragir.pro file.")

TEMPLATE = app
TARGET = oragir

# Will build the final executable in the release directory.
DESTDIR = ../release

QT += network xml sql webkit
CONFIG += release
win32:CONFIG += embed_manifest_exe

INCLUDEPATH += GeneratedFiles \
               GeneratedFiles/release \
               .
win32:LIBS += -lqtmain -lQtCore4 -lQtGui4 -lQtNetwork4 -lQtSql -l../release/ljcommunicator
unix:LIBS += -lQtCore -lQtGui -lQtNetwork -lQtSql -L../release -lljcommunicator

DEPENDPATH += .
MOC_DIR += GeneratedFiles/release
UI_DIR += GeneratedFiles
RCC_DIR += GeneratedFiles
OBJECTS_DIR += release

# Adds icon to the executable file on Windows.
win32:RC_FILE = application.rc

#Include file(s)
! include( oragir.pri ) {
    error( Couldn't find the oragir.pri file! )
}
