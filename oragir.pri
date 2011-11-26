#Header files
HEADERS += ./core/propertyTable.h \
           ./core/application.h \
           ./core/defaultManager.h \
           ./gui/mainWindow.h \
           ./gui/databaseView.h \
           ./gui/dlgFilters.h \
           ./gui/dlgFilterEditor.h \
           ./gui/previewWindow.h \
           ./parser/atomParser.h \
           ./parser/blog.h \
           ./parser/post.h \
           ./parser/filter.h \
           ./database/database.h \
           ./strings/strings.h \
           ./strings/guiStrings.h

#Source files
SOURCES += ./main.cpp \
           ./core/propertyTable.cpp \
           ./core/application.cpp \
           ./core/defaultManager.cpp \
           ./gui/mainWindow.cpp \
           ./gui/databaseView.cpp \
           ./gui/dlgFilters.cpp \
           ./gui/dlgFilterEditor.cpp \
           ./gui/previewWindow.cpp \
           ./parser/blog.cpp \
           ./parser/post.cpp \
           ./database/database.cpp \
           ./parser/atomParser.cpp \
           ./strings/strings.cpp \
           ./strings/guiStrings.cpp

#Resource file(s)
RESOURCES += ./resources/oragir.qrc
