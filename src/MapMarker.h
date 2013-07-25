/*****************************************************************************
 * MapMarker.h
 *
 * Created: 07/7 2013 by uranusjr
 *
 * Copyright 2013 uranusjr. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
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
