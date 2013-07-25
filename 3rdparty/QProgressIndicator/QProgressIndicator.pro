TEMPLATE = lib
QT += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
}

CONFIG += staticlib

DESTDIR = ../../lib

# Input
HEADERS += QProgressIndicator.h
SOURCES += QProgressIndicator.cpp
