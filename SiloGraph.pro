#-------------------------------------------------
#
# Project created by QtCreator 2013-07-04T15:45:35
#
#-------------------------------------------------

QT       += core gui sql webkit

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = SiloGraph
TEMPLATE = app

CONFIG += qwt

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
    DatabaseConnector.cpp \
    NodeData.cpp \
    MapContainer.cpp \
    MapMarker.cpp \
    LogoHolder.cpp

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
    DatabaseConnector.h \
    NodeData.h \
    MapContainer.h \
    MapMarker.h \
    LogoHolder.h

OTHER_FILES += \
    locations.json \
    README.md \
    databases.json \
    map.html \
    map.js

RESOURCES += \
    Resources.qrc
