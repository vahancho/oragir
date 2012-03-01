#Header files
HEADERS += ./core/propertyTable.h \
           ./core/application.h \
           ./core/credentials.h \
           ./core/defaultManager.h \
           ./core/versionManager.h \
           ./core/version.h \
           ./gui/mainWindow.h \
           ./gui/databaseView.h \
           ./gui/dlgFilters.h \
           ./gui/dlgFilterEditor.h \
           ./gui/previewWindow.h \
           ./gui/postTableModel.h \
           ./gui/abstractOptionsPage.h \
           ./gui/generalOptionsPage.h \
           ./gui/advancedOptionsPage.h \
           ./gui/connectOptionsPage.h \
           ./gui/dlgOptions.h \
           ./gui/dlgUser.h \
           ./gui/htmlEditor.h \
           ./gui/blogTableModel.h \
           ./gui/blogEventView.h \
           ./gui/imagedownloader.h \
           ./gui/staticMenu.h \
           ./gui/comboBox.h \
           ./parser/atomParser.h \
           ./parser/blog.h \
           ./parser/post.h \
           ./parser/filter.h \
           ./database/database.h \
           ./database/blogdatabase.h \
           ./database/streamdatabase.h \
           ./strings/strings.h \
           ./strings/guiStrings.h

#Source files
SOURCES += ./main.cpp \
           ./core/propertyTable.cpp \
           ./core/application.cpp \
           ./core/credentials.cpp \
           ./core/defaultManager.cpp \
           ./core/versionManager.cpp \
           ./core/version.cpp \
           ./gui/mainWindow.cpp \
           ./gui/databaseView.cpp \
           ./gui/dlgFilters.cpp \
           ./gui/dlgFilterEditor.cpp \
           ./gui/previewWindow.cpp \
           ./gui/postTableModel.cpp \
           ./gui/generalOptionsPage.cpp \
           ./gui/advancedOptionsPage.cpp \
           ./gui/connectOptionsPage.cpp \
           ./gui/dlgOptions.cpp \
           ./gui/dlgUser.cpp \
           ./gui/htmlEditor.cpp \
           ./gui/blogTableModel.cpp \
           ./gui/blogEventView.cpp \
           ./gui/imagedownloader.cpp \
           ./gui/staticMenu.cpp \
           ./gui/comboBox.cpp \
           ./parser/blog.cpp \
           ./parser/post.cpp \
           ./database/database.cpp \
           ./database/blogdatabase.cpp \
           ./database/streamdatabase.cpp \
           ./parser/atomParser.cpp \
           ./strings/strings.cpp \
           ./strings/guiStrings.cpp

#Resource file(s)
RESOURCES += ./resources/oragir.qrc
