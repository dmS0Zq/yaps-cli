TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Entry.cpp \
    DateTime.cpp \
    Database.cpp \
    FileIO.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    LinkedListNode.h \
    Tree.h \
    Entry.h \
    DateTime.h \
    Database.h \
    FileIO.h

CONFIG += \
    c++11

OTHER_FILES += \
    FileFormat.txt
