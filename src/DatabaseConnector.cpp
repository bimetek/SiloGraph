/*****************************************************************************
 * DatabaseConnector.cpp
 *
 * Created: 05/7 2013 by uranusjr
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

#include "DatabaseConnector.h"
#if QT_VERSION >= 0x050000
    #include <QtConcurrent/QtConcurrent>
#else
    #include <QtConcurrentRun>
#endif
#include <QFile>
#include <QFuture>
#include <QFutureWatcher>
#include <QMutexLocker>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QVariantMap>
#if QT_VERSION >= 0x050000
    #include <QJsonDocument>
#else
    #include <qjson/parser.h>
#endif
#include "Globals.h"
#include "Location.h"
#include "Silo.h"
#include "NodeLine.h"
#include "Node.h"
#include "NodeData.h"
#include <QDebug>

Queryable::Context executeNodeQuery(Silo *silo, Node *node, QMutex *m)
{
    if (node)
        return node->executeWeekDataFetch(m);
    else
        return silo->executeWeekDataFetch(m);
}

QList<Queryable::Context> executePollForLocation(Location *location, QMutex *m)
{
    QString address = location->databaseAddress();
    QSqlDatabase db = QSqlDatabase::database(address);

    QList<Queryable::Context> contexts;
    foreach (Silo *silo, location->silos())
    {
        foreach (NodeLine *line, silo->lines())
            contexts.append(line->executePoll(m, false));
    }

    db.close();
    return contexts;
}

DatabaseConnector::DatabaseConnector(QObject *parent) :
    QObject(parent)
{
    QFile dbFile("databases.json");

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
    QJsonObject dbs = doc.object()["databases"].toObject();
    foreach (QString key, dbs.keys())
    {
        QJsonObject info = dbs[key].toObject();
        QString address = info["address"].toString();
        _databaseMutexes.insert(address, new QMutex());
        QSqlDatabase db = QSqlDatabase::addDatabase(info["driver"].toString(),
                                                    address);
        db.setHostName(address);
        db.setUserName(info["username"].toString());
        db.setPassword(info["password"].toString());
        db.setDatabaseName("almin");
        if (info.contains("port"))
            db.setPort(info["port"].toDouble());
    }
#else
    QJson::Parser parser;
    bool success = false;
    QVariant result = parser.parse(&dbFile, &success);

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
        _databaseMutexes.insert(address, new QMutex());
        QSqlDatabase db = QSqlDatabase::addDatabase(info["driver"].toString(),
                                                    address);
        db.setHostName(address);
        db.setUserName(info["username"].toString());
        db.setPassword(info["password"].toString());
        db.setDatabaseName("almin");
        if (info.contains("port"))
            db.setPort(info["port"].toInt());
    }
#endif
}

DatabaseConnector::~DatabaseConnector()
{
    foreach (QString name, QSqlDatabase::connectionNames())
    {
        QSqlDatabase::removeDatabase(name);
        _databaseMutexes.remove(name);
    }
}

void DatabaseConnector::fetchWeekData(Node *node, Silo *silo)
{
    QMutex *m = _databaseMutexes[silo->location()->databaseAddress()];
    QFuture<Queryable::Context> future =
            QtConcurrent::run(executeNodeQuery, silo, node, m);
    QFutureWatcher<Queryable::Context> *watcher =
            new QFutureWatcher<Queryable::Context>();
    connect(watcher, SIGNAL(finished()), this, SLOT(processWeekDataQuery()));
    watcher->setFuture(future);
    emit fetchingStarted(node, silo);
}

void DatabaseConnector::processWeekDataQuery()
{
    QFutureWatcher<Queryable::Context> *watcher =
            reinterpret_cast<QFutureWatcher<Queryable::Context> *>(sender());
    watcher->deleteLater();

    Queryable::Context context = watcher->result();

    QSqlQuery &query = context.query;
    if (!query.size())
        return;

    Node *node = dynamic_cast<Node *>(context.entity);
    Silo *silo = 0;
    if (node)   // Try to make entity a node
        silo = node->line()->silo();
    else        // Try to make entity a silo instead
        silo = dynamic_cast<Silo *>(context.entity);
    if (!silo)  // entity is something unrecognizable
        return;

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
    QMutex *m = _databaseMutexes[location->databaseAddress()];
    QFuture< QList<Queryable::Context> > future =
            QtConcurrent::run(executePollForLocation, location, m);
    QFutureWatcher< QList<Queryable::Context> > *watcher =
            new QFutureWatcher< QList<Queryable::Context> >();
    connect(watcher, SIGNAL(finished()), this, SLOT(processLatestDataQuery()));
    watcher->setFuture(future);
}

void DatabaseConnector::processLatestDataQuery()
{
    typedef QList<Queryable::Context> ContextList;
    QFutureWatcher<ContextList> *watcher =
            reinterpret_cast<QFutureWatcher<ContextList> *>(sender());
    watcher->deleteLater();

    QList<Queryable::Context> contexts = watcher->result();

    foreach (Queryable::Context context, contexts)
    {
        QSqlQuery &query = context.query;
        bool toLast = query.last();
        if (toLast)    // Successfully moved cursor to last row
        {
            QList<double> temperatures;
            NodeLine *line = dynamic_cast<NodeLine *>(context.entity);
            if (!line)
                continue;
            for (int i = 0; i < line->nodes().size(); i++)
            {
                bool ok = false;
                double temperature = query.value(i + 1).toDouble(&ok);
                if (!ok)
                    temperature = D_NO_DATA;
                temperatures.append(temperature);
            }
            QDateTime dateTime = query.value(0).toDateTime();
            emit dataPolled(line, temperatures, dateTime);
        }
    }
}
