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
#include <QtConcurrentRun>
#include <QFile>
#include <QFuture>
#include <QFutureWatcher>
#include <QMutexLocker>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariantMap>
#include <qjson/parser.h>
#include "Globals.h"
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

struct LineQueryContext
{
    QSqlQuery query;
    NodeLine *line;
};

NodeQueryContext executeNodeQuery(Silo *silo, Node *node, QMutex *m)
{
    QMutexLocker locker(m);
    Q_UNUSED(locker);

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
                "WHERE silo_cable = \"%2\" AND date > \"%3\" "
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
                "WHERE silo = \"%1\" AND date > \"%2\" "
                "ORDER BY date DESC";
        queryString = queryFormat.arg(silo->name(), oneWeekAgo);
        dataCount = 2;
    }

    NodeQueryContext context;
    context.query = db.exec(queryString);
    context.silo = silo;
    context.node = node;
    context.dataCount = dataCount;

    db.close();
    return context;
}

QList<LineQueryContext> executePollForLocation(Location *location, QMutex *m)
{
    QMutexLocker locker(m);
    Q_UNUSED(locker);

    QString address = location->databaseAddress();
    QSqlDatabase db = QSqlDatabase::database(address);

    QList<LineQueryContext> contexts;
    foreach (Silo *silo, location->silos())
    {
        foreach (NodeLine *line, silo->lines())
        {
            QString queryStringFormat =
                    "SELECT date, %1 FROM rawdata WHERE silo_cable = \"%2\" "
                    "ORDER BY date DESC LIMIT 1";
            QStringList names;
            foreach (Node *node, line->nodes())
                names << node->name();
            QString queryString = queryStringFormat.arg(names.join(", "),
                                                        line->name());
            LineQueryContext context;
            context.query = db.exec(queryString);
            context.line = line;
            contexts.append(context);
        }
    }

    db.close();
    return contexts;
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

DatabaseConnector::~DatabaseConnector()
{
    foreach (QString name, QSqlDatabase::connectionNames())
        QSqlDatabase::removeDatabase(name);
}

void DatabaseConnector::fetchWeekData(Node *node, Silo *silo)
{
    QFuture<NodeQueryContext> future =
            QtConcurrent::run(executeNodeQuery, silo, node, &_databaseMutex);
    QFutureWatcher<NodeQueryContext> *watcher =
            new QFutureWatcher<NodeQueryContext>();
    connect(watcher, SIGNAL(finished()), this, SLOT(processWeekDataQuery()));
    watcher->setFuture(future);
}

void DatabaseConnector::processWeekDataQuery()
{
    QFutureWatcher<NodeQueryContext> *watcher =
            reinterpret_cast<QFutureWatcher<NodeQueryContext> *>(sender());
    watcher->deleteLater();

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
            bool ok = false;
            double temperature = query.value(i + 1).toDouble(&ok);
            if (!ok)
                continue;
            NodeData *data = new NodeData(this);
            data->setDateTime(dateTime);
            data->setTemperature(temperature);
            dataSets[i].append(data);
        }
    }
    emit dataFetched(node, silo, dataSets);
}

void DatabaseConnector::fetchLatestData(Location *location)
{
    QFuture< QList<LineQueryContext> > future =
            QtConcurrent::run(executePollForLocation, location, &_databaseMutex);
    QFutureWatcher< QList<LineQueryContext> > *watcher =
            new QFutureWatcher< QList<LineQueryContext> >();
    connect(watcher, SIGNAL(finished()), this, SLOT(processLatestDataQuery()));
    watcher->setFuture(future);
}

void DatabaseConnector::processLatestDataQuery()
{
    QFutureWatcher< QList<LineQueryContext> > *watcher =
        reinterpret_cast<QFutureWatcher< QList<LineQueryContext> > *>(sender());
    watcher->deleteLater();

    QList<LineQueryContext> contexts = watcher->result();

    foreach (LineQueryContext context, contexts)
    {
        QSqlQuery &query = context.query;
        bool toLast = query.last();
        if (toLast)    // Successfully moved cursor to last row
        {
            QList<double> temperatures;
            for (int i = 0; i < context.line->nodes().size(); i++)
            {
                bool ok = false;
                double temperature = query.value(i + 1).toDouble(&ok);
                if (!ok)
                    temperature = D_NO_DATA;
                temperatures.append(temperature);
            }
            QDateTime dateTime = query.value(0).toDateTime();
            emit dataPolled(context.line, temperatures, dateTime);
        }
    }
}
