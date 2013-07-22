/*****************************************************************************
 * MapContainer.cpp
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

#include "MapContainer.h"
#include <QFile>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QVariantMap>
#if QT_VERSION >= 0x050000
    #include <QJsonDocument>
    #include <QJsonObject>
    #include <QJsonArray>
#else
    #include <qjson/parser.h>
#endif
#include "Globals.h"
#include "MapView.h"
#include "MapMarker.h"
#include "Location.h"
#include "Silo.h"
#include "NodeLine.h"
#include "Node.h"
#include <QDebug>

MapContainer::MapContainer(QWidget *parent) :
    QWidget(parent), _currentLocation(0)
{
    _mapView = new MapView();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(_mapView);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    connect(_mapView, SIGNAL(mapLoaded()), this, SLOT(loadMapContents()));
    connect(_mapView, SIGNAL(markerClicked(MapMarker *)),
            this, SLOT(switchToMarkerLocation(MapMarker *)));

    QFile dbFile("databases.json");
    QFile json("locations.json");


#if QT_VERSION >= 0x050000
    dbFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(dbFile.readAll(), &parseError);
    dbFile.close();
    if (parseError.error != QJsonParseError::NoError)
    {
        qFatal(parseError.errorString().toUtf8().data());
        return;
    }
    QJsonObject dbInfo = doc.object();
    QString defaultAddress = dbInfo["default"].toString();

    json.open(QIODevice::ReadOnly | QIODevice::Text);
    doc = QJsonDocument::fromJson(json.readAll(), &parseError);
    json.close();
    if (parseError.error != QJsonParseError::NoError)
    {
        qFatal(parseError.errorString().toUtf8().data());
        return;
    }
    QJsonObject result = doc.object();
    foreach(QJsonValue item, result["locations"].toArray())
    {
        QJsonObject info = item.toObject();
        QString address = defaultAddress;
        if (info.contains("databaseAddress"))
            address = info["databaseAddress"].toString();

        Location *location = new Location(this);
        location->setName(info["name"].toString());
        location->setDatabaseAddress(address);
        location->setLatitude(info["latitude"].toDouble());
        location->setLongitude(info["longitude"].toDouble());
        foreach (QJsonValue siloItem, info["silos"].toArray())
        {
            Silo *silo = new Silo(location);
            info = siloItem.toObject();
            silo->setName(info["name"].toString());
            foreach (QJsonValue lineItem, info["lines"].toArray())
            {
                NodeLine *line = new NodeLine(silo);
                info = lineItem.toObject();
                line->setName(info["name"].toString());
                for (int i = 0; i < info["nodesCount"].toDouble(); i++)
                {
                    Node *node = new Node(line);
                    node->setName(QString("s%1").arg(QString::number(i + 1)));
                    line->addNode(node);
                }
                silo->addLine(line);
            }
            location->addSilo(silo);
        }
        _locations.insert(location->name(), location);
    }
#else
    QJson::Parser parser;
    bool success = false;

    QVariantMap dbInfo = parser.parse(&dbFile, &success).toMap();
    if (!success)
    {
        qFatal("Database JSON cannot be parsed");
        return;
    }
    QString defaultAddress = dbInfo["default"].toString();

    QVariantMap result = parser.parse(&json, &success).toMap();
    if (!success)
    {
        qFatal("Location JSON cannot be parsed");
        return;
    }

    foreach(QVariant item, result["locations"].toList())
    {
        QVariantMap info = item.toMap();
        QString address = defaultAddress;
        if (info.contains("databaseAddress"))
            address = info["databaseAddress"].toString();

        Location *location = new Location(this);
        location->setName(info["name"].toString());
        location->setDatabaseAddress(address);
        location->setLatitude(info["latitude"].toDouble());
        location->setLongitude(info["longitude"].toDouble());
        foreach (QVariant siloItem, info["silos"].toList())
        {
            Silo *silo = new Silo(location);
            info = siloItem.toMap();
            silo->setName(info["name"].toString());
            foreach (QVariant lineItem, info["lines"].toList())
            {
                NodeLine *line = new NodeLine(silo);
                info = lineItem.toMap();
                line->setName(info["name"].toString());
                for (int i = 0; i < info["nodesCount"].toInt(); i++)
                {
                    Node *node = new Node(line);
                    node->setName(QString("s%1").arg(QString::number(i + 1)));
                    line->addNode(node);
                }
                silo->addLine(line);
            }
            location->addSilo(silo);
        }
        _locations.insert(location->name(), location);
    }
#endif
    setMinimumWidth(100);
}

void MapContainer::switchToMarkerLocation(MapMarker *marker)
{
    QString name = marker->name();
    if (_locations.contains(name))
    {
        Location *nextLocation = _locations[name];
        if (_currentLocation != nextLocation)
        {
            _currentLocation = nextLocation;
            emit locationChanged(_locations[name]);
        }
    }
}

void MapContainer::loadMapContents()
{
    foreach (Location *location, _locations)
    {
        MapMarker *marker = new MapMarker(this);
        marker->setName(location->name());
        marker->setLatitude(location->latitude());
        marker->setLongitude(location->longitude());
        _mapView->appendMarker(marker);
    }
}
