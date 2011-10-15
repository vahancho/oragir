#Header files
HEADERS += ./parser/atomParser.h \
           ./parser/blog.h \
           ./parser/post.h \
           ./database/database.h \
           ./strings/strings.h \
           ./strings/guiStrings.h

#Source files
SOURCES += ./main.cpp \
           ./parser/blog.cpp \
           ./parser/post.cpp \
           ./database/database.cpp \
           ./parser/atomParser.cpp \
           ./strings/strings.cpp \
           ./strings/guiStrings.cpp

#Resource file(s)
RESOURCES += ./resources/oragir.qrc
