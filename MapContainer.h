/*****************************************************************************
 * MapContainer.h
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

#ifndef MAPCONTAINER_H
#define MAPCONTAINER_H

#include <QWidget>
#include <QHash>
class MapMarker;
class MapView;
class Location;

class MapContainer : public QWidget
{
    Q_OBJECT

public:
    explicit MapContainer(QWidget *parent = 0);

signals:
    void locationChanged(Location *location);

private slots:
    void loadMapContents();
    void switchToMarkerLocation(MapMarker *marker);

private:
    QHash<QString, Location *> _locations;
    MapView *_mapView;
};

#endif // MAPCONTAINER_H
