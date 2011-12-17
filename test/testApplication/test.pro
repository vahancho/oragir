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
          ../../core/versionManager.cpp \
          ../../core/version.cpp \
          ../../gui/mainWindow.cpp \
          ../../gui/databaseView.cpp \
          ../../gui/dlgFilters.cpp \
          ../../gui/dlgFilterEditor.cpp \
          ../../gui/dlgOptions.cpp \
          ../../gui/connectOptionsPage.cpp \
          ../../gui/advancedOptionsPage.cpp \
          ../../gui/generalOptionsPage.cpp \
          ../../gui/databaseView.cpp \
          ../../gui/postTableModel.cpp \
          ../../gui/previewWindow.cpp \
          ../../core/propertyTable.cpp \
          ../../parser/post.cpp \
          ../../parser/blog.cpp \
          ../../strings/strings.cpp \
          ../../strings/guiStrings.cpp \
          ../../GeneratedFiles/release/moc_database.cpp \
          ../../GeneratedFiles/release/moc_atomParser.cpp \
          ../../GeneratedFiles/release/moc_defaultManager.cpp \
          ../../GeneratedFiles/release/moc_propertyTable.cpp \
          ../../GeneratedFiles/release/moc_mainWindow.cpp \
          ../../GeneratedFiles/release/moc_dlgFilters.cpp \
          ../../GeneratedFiles/release/moc_dlgFilterEditor.cpp \
          ../../GeneratedFiles/release/moc_databaseView.cpp \
          ../../GeneratedFiles/release/moc_versionManager.cpp \
          ../../GeneratedFiles/release/moc_dlgOptions.cpp \
          ../../GeneratedFiles/release/moc_generalOptionsPage.cpp \
          ../../GeneratedFiles/release/moc_advancedOptionsPage.cpp \
          ../../GeneratedFiles/release/moc_connectOptionsPage.cpp \
          ../../GeneratedFiles/release/moc_abstractOptionsPage.cpp \
          ../../GeneratedFiles/release/moc_databaseView.cpp \
          ../../GeneratedFiles/release/moc_previewWindow.cpp
