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
#include <QtConcurrentRun>
#include <QFile>
#include <QFuture>
#include <QFutureWatcher>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariantMap>
#include <qjson/parser.h>
#include "Location.h"
#include "Silo.h"
#include "NodeLine.h"
#include "Node.h"
#include "NodeData.h"
#include <QDebug>

struct NodeQueryContext
{
    QSqlQuery query;
    Silo *silo;
    Node *node;
    uint dataCount;
};

NodeQueryContext executeNodeQuery(QSqlDatabase &db, QString &queryString,
                                  Silo *silo, Node *node, uint dataCount)
{
    NodeQueryContext context;
    context.query = db.exec(queryString);
    context.silo = silo;
    context.node = node;
    context.dataCount = dataCount;
    return context;
}

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

void DatabaseConnector::fetchWeekData(Node *node, Silo *silo)
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
    QFuture<NodeQueryContext> future = QtConcurrent::run(
                executeNodeQuery, db, queryString, silo, node, dataCount);
    QFutureWatcher<NodeQueryContext> *watcher =
            new QFutureWatcher<NodeQueryContext>();
    connect(watcher, SIGNAL(finished()), this, SLOT(processWeekDataQuery()));
    watcher->setFuture(future);
}

void DatabaseConnector::processWeekDataQuery()
{
    QFutureWatcher<NodeQueryContext> *watcher =
            reinterpret_cast<QFutureWatcher<NodeQueryContext> *>(sender());

    NodeQueryContext context = watcher->result();

    QSqlQuery &query = context.query;
    if (!query.size())
        return;

    Silo *&silo = context.silo;
    Node *&node = context.node;
    uint &dataCount = context.dataCount;
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

void DatabaseConnector::fetchLatestData(Location *location)
{
//    QString address = location->databaseAddress();
//    QSqlDatabase db = QSqlDatabase::database(address);

//    QStringList names;
//    foreach (Silo *silo, location->silos())
//    {
//        foreach (NodeLine *line, silo->lines())
//            names << QString("\"%1\"").arg(line->name());
//    }
//    QString lineNames = names.join(", ");
//    QString query =
//            "SELECT date, silo_cable, %1 FROM"
}
