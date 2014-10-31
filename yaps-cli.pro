TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Entry.cpp \
    DateTime.cpp \
    FileIO.cpp \
    Database.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Entry.h \
    FileIO.h \
    DateTime.h \
    Tree.h \
    Database.h

CONFIG += \
    c++11

OTHER_FILES += \
    FileFormat.txt
