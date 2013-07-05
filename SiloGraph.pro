#-------------------------------------------------
#
# Project created by QtCreator 2013-07-04T15:45:35
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SiloGraph
TEMPLATE = app

LIBS += -lqjson

SOURCES += main.cpp\
        MainWindow.cpp \
    MainWidget.cpp \
    MapView.cpp \
    SiloListView.cpp \
    GraphContainer.cpp \
    Location.cpp \
    Silo.cpp \
    Node.cpp \
    NodeLine.cpp \
    SiloView.cpp \
    Globals.cpp \
    DatabaseConnector.cpp

HEADERS  += MainWindow.h \
    MainWidget.h \
    Globals.h \
    MapView.h \
    SiloListView.h \
    GraphContainer.h \
    Location.h \
    Silo.h \
    Node.h \
    NodeLine.h \
    SiloView.h \
    DatabaseConnector.h

OTHER_FILES += \
    locations.json \
    README.md \
    databases.json

RESOURCES += \
    Resources.qrc
