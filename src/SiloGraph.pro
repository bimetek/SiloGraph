#-------------------------------------------------
#
# Project created by QtCreator 2013-07-04T15:45:35
#
#-------------------------------------------------

QT       += core gui sql webkit

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets webkitwidgets
    LIBS += -lQt5Concurrent
}
!greaterThan(QT_MAJOR_VERSION, 4) {
    LIBS += -lqjson
}


TARGET = SiloGraph
TEMPLATE = app

CONFIG += qwt

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
    LogoHolder.cpp \
    SharedSettings.cpp \
    SettingsWindow.cpp

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
    LogoHolder.h \
    SharedSettings.h \
    SettingsWindow.h

OTHER_FILES += \
    locations.json \
    README.md \
    databases.json \
    map.html \
    map.js \
    logo.png \
    logo_transparent.png \
    silo_background.png

RESOURCES += \
    Resources.qrc
