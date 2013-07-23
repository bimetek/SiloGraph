/*****************************************************************************
 * MapMarker.h
 *
 * Created: 07/7 2013 by uranusjr
 *
 * Copyright 2013 uranusjr. All rights reserved.
 *
 * This file is published under the Creative Commons 3.0.
 * http://creativecommons.org/licenses/by/3.0/
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/

#ifndef MAPMARKER_H
#define MAPMARKER_H

#include <QObject>

class MapMarker : public QObject
{
    Q_OBJECT

public:
    explicit MapMarker(QObject *parent = 0);

private:
    QString _name;
    double _latitude;
    double _longitude;

public:
    inline QString name() { return _name; }
    inline void setName(QString name) { _name = name; }
    inline double latitude() { return _latitude; }
    inline void setLatitude(double latitude) { _latitude = latitude; }
    inline double longitude() { return _longitude; }
    inline void setLongitude(double longitude) { _longitude = longitude; }
};

#endif // MAPMARKER_H
