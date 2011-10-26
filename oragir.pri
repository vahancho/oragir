#Header files
HEADERS += ./core/propertyTable.h \
           ./core/application.h \
           ./core/defaultManager.h \
           ./gui/mainWindow.h \
           ./parser/atomParser.h \
           ./parser/blog.h \
           ./parser/post.h \
           ./parser/rule.h \
           ./database/database.h \
           ./strings/strings.h \
           ./strings/guiStrings.h

#Source files
SOURCES += ./main.cpp \
           ./core/propertyTable.cpp \
           ./core/application.cpp \
           ./core/defaultManager.cpp \
           ./gui/mainWindow.cpp \
           ./parser/blog.cpp \
           ./parser/post.cpp \
           ./database/database.cpp \
           ./parser/atomParser.cpp \
           ./strings/strings.cpp \
           ./strings/guiStrings.cpp

#Resource file(s)
RESOURCES += ./resources/oragir.qrc
