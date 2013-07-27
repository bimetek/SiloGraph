TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS = QProgressIndicator

!greaterThan(QT_MAJOR_VERSION, 4): SUBDIRS += qjson-backport
