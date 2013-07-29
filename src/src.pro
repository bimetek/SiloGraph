#-------------------------------------------------
#
# Project created by QtCreator 2013-07-04T15:45:35
#
#-------------------------------------------------

QT       += core gui sql webkit

unix|win32-g++:LIBS += -L../lib

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets webkitwidgets
}
!greaterThan(QT_MAJOR_VERSION, 4) {
    INCLUDEPATH += ../3rdparty/qjson-backport
    unix|win32-g++:LIBS += -lqjsonbackport
    else:LIBS += ../lib/qjsonbackport.lib
}

TARGET = SiloGraph
TEMPLATE = app

DESTDIR = ../../bin

CONFIG += qwt

INCLUDEPATH += ../3rdparty

unix|win32-g++:LIBS += -lQProgressIndicator
else:LIBS += ../lib/QProgressIndicator.lib

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
    SettingsWindow.cpp \
    ProgressLayer.cpp \
    Queryable.cpp \
    CurveTrackingPicker.cpp

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
    SettingsWindow.h \
    ProgressLayer.h \
    Queryable.h \
    CurveTrackingPicker.h

OTHER_FILES += \
    locations.json \
    README.md \
    databases.json \
    map.html \
    map.js \
    logo.png \
    logo_transparent.png \
    silo_background.png \
    silo_button_styles.css

RESOURCES += \
    Resources.qrc
