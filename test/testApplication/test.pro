QT += testlib network xml sql
CONFIG += console debug_and_release
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .\
               $(QTDIR)/include/QtNetwork \
               $(QTDIR)/include/QtSql

# Input
HEADERS +=
SOURCES += testApplication.cpp \
           ../../core/application.cpp \
	   ../../database/database.cpp \
	   ../../parser/atomParser.cpp \
	   ../../core/defaultManager.cpp \
	   ../../gui/mainWindow.cpp \
	   ../../core/propertyTable.cpp \
	   ../../parser/post.cpp \
	   ../../parser/blog.cpp \
	   ../../strings/strings.cpp \
	   ../../strings/guiStrings.cpp \
	   ../../GeneratedFiles/debug/moc_database.cpp \
	   ../../GeneratedFiles/debug/moc_atomParser.cpp \
	   ../../GeneratedFiles/debug/moc_defaultManager.cpp \
	   ../../GeneratedFiles/debug/moc_propertyTable.cpp \
	   ../../GeneratedFiles/debug/moc_mainWindow.cpp
