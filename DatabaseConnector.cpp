/*****************************************************************************
 * DatabaseConnector.cpp
 *
 * Created: 05/7 2013 by uranusjr
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

#include "DatabaseConnector.h"
#include <limits>
#include <QFile>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariantMap>
#include <qjson/parser.h>
#include "Location.h"
#include "Silo.h"
#include "NodeLine.h"
#include "Node.h"
#include "NodeData.h"
#include <QDebug>

DatabaseConnector::DatabaseConnector(QObject *parent) :
    QObject(parent)
{
    QJson::Parser parser;
    bool success = false;
    QFile *dbFile = new QFile(":/data/databases.json");
    QVariant result = parser.parse(dbFile, &success);
    delete dbFile;

    if (!success)
    {
        qFatal("Database JSON cannot be parsed");
        return;
    }
    QVariantMap dbs = result.toMap()["databases"].toMap();
    foreach (QString key, dbs.keys())
    {
        QVariantMap info = dbs[key].toMap();
        QString address = info["address"].toString();
        QSqlDatabase db = QSqlDatabase::addDatabase(info["driver"].toString(),
                                                    address);
        db.setHostName(address);
        db.setUserName(info["username"].toString());
        db.setPassword(info["password"].toString());
        db.setDatabaseName("almin");
        if (info.contains("port"))
            db.setPort(info["port"].toInt());
    }
}

void DatabaseConnector::fetchData(Node *node, Silo *silo)
{
    QString address = silo->location()->databaseAddress();
    QSqlDatabase db = QSqlDatabase::database(address);

    QString queryString;
    QDateTime now = QDateTime::currentDateTime();
    QString oneWeekAgo = now.addDays(-7).toString(Qt::ISODate);
    uint dataCount = 0;
    if (node)
    {
        QString queryFormat =
                "SELECT date, %1 FROM rawdata "
                "WHERE silo_cable=\"%2\" AND date > \"%3\" "
                "ORDER BY date DESC";
        queryString = queryFormat.arg(
                    node->name(),
                    node->line()->name(),
                    oneWeekAgo);
        dataCount = 1;
    }
    else
    {
        QString queryFormat =
                "SELECT date, Full, Empty FROM average "
                "WHERE silo=\"%1\" AND date > \"%2\" "
                "ORDER BY date DESC";
        queryString = queryFormat.arg(silo->name(), oneWeekAgo);
        dataCount = 2;
    }
    QSqlQuery query = db.exec(queryString);
    if (query.size())
    {
        QList< QList<NodeData *> > dataSets;
        for (uint i = 0; i < dataCount; i++)
        {
            QList<NodeData *> dataSet;
            dataSets.append(dataSet);
        }
        while (query.next())
        {
            QDateTime dateTime = query.value(0).toDateTime();
            for (uint i = 0; i < dataCount; i++)
            {
                NodeData *data = new NodeData(this);
                data->setDateTime(dateTime);
                bool ok = false;
                double temperature = query.value(i + 1).toDouble(&ok);
                if (!ok)
                    temperature = -1 * std::numeric_limits<double>::max();
                data->setTemperature(temperature);
                dataSets[i].append(data);
            }
        }
        emit dataFetched(node, silo, dataSets);
    }
}
