LIBS += -lQtTest
CONFIG += testlib console debug_and_release
TEMPLATE = app
TARGET =
DEPENDPATH += .
INCLUDEPATH += .

# Input
SOURCES += testFilter.cpp \
           ../../parser/post.cpp \
           ../../strings/strings.cpp \
           ../../core/propertyTable.cpp \
           ../../GeneratedFiles/debug/moc_propertyTable.cpp
