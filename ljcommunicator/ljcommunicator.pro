message("You are running qmake on ljcommunicator.pro file.")

TEMPLATE = lib
TARGET = ljcommunicator
DESTDIR = ./release
QT += network xml

CONFIG += release
win32:CONFIG += embed_manifest_exe

INCLUDEPATH += ./GeneratedFiles \
               ./GeneratedFiles/release \
			   $(ProjectDir)/. \
               .
win32:LIBS += -lqtmain -lQtCore4 -lQtNetwork4
unix:LIBS += -lQtCore-lQtNetwork

win32:DEFINES += LJCOMMUNICATOR_LIB

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles

HEADERS += export.h communicator.h ljevents.h ljsyncitems.h ljuserinfo.h \
           ljusertags.h request.h response.h
SOURCES += communicator.cpp ljevents.cpp ljsyncitems.cpp ljuserinfo.cpp \
           ljusertags.cpp request.cpp response.cpp

