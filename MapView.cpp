/*****************************************************************************
 * MapView.cpp
 *
 * Created: 04/7 2013 by uranusjr
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

#include "MapView.h"
#include <QFile>
#include <QPushButton>
#include <QSignalMapper>
#include <QVariantMap>
#include <QDebug>
#include <qjson/parser.h>
#include "Globals.h"
#include "Location.h"
#include "Silo.h"
#include "NodeLine.h"
#include "Node.h"

MapView::MapView(QWidget *parent) :
    QWidget(parent)
{
    setFixedWidth(300 * W_SCALE);
    QPalette p(palette());
    p.setColor(QPalette::Background, Qt::white);
    setPalette(p);
    setAutoFillBackground(true);

    QJson::Parser parser;
    bool success = false;

    QFile *dbFile = new QFile(":/data/databases.json");
    QVariantMap dbInfo = parser.parse(dbFile, &success).toMap();
    if (!success)
    {
        qFatal("Database JSON cannot be parsed");
        return;
    }
    QString defaultAddress = dbInfo["default"].toString();
    delete dbFile;

    QFile *json = new QFile(":/data/locations.json");
    QVariantMap result = parser.parse(json, &success).toMap();
    if (!success)
    {
        qFatal("Location JSON cannot be parsed");
        return;
    }

    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(QString)),
            this, SLOT(switchToLocation(QString)));
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

        QPushButton *button = new QPushButton(location->name(), this);
        button->move((location->longitude() - 121.0) * 100.0,
                     (location->latitude() - 24.0) * 100.0);

        connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
        mapper->setMapping(button, location->name());
    }
    delete json;
}

void MapView::switchToLocation(QString title)
{
    emit locationChanged(_locations[title]);
}
