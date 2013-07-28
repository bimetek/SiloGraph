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
    #include <QtConcurrent/QtConcurrentRun>
#else
    #include <QtConcurrentRun>
#endif
#include <QFile>
#include <QFutureWatcher>
#include <QStringList>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include "Globals.h"
#include "Location.h"
#include "Silo.h"
#include "NodeLine.h"
#include "Node.h"
#include "NodeData.h"

static Queryable::Context executeWeekDataQuery(Queryable *entity,
                                               QString key, QMutex *mutex)
{
    return entity->executeWeekDataFetch(mutex, key);
}

static QList<Queryable::Context> executePoll(Location *location, QMutex *mutex)
{
    QString address = location->databaseAddress();
    QSqlDatabase db = QSqlDatabase::database(address);

    QList<Queryable::Context> contexts;
    contexts.append(location->executePoll(mutex, false));
    foreach (Silo *silo, location->silos())
    {
        foreach (NodeLine *line, silo->lines())
            contexts.append(line->executePoll(mutex, false));
    }

    db.close();
    return contexts;
}

DatabaseConnector::DatabaseConnector(QObject *parent) :
    QObject(parent)
{
    QFile dbFile("databases.json");

    dbFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(dbFile.readAll(), &parseError);
    dbFile.close();
    if (parseError.error != QJsonParseError::NoError)
    {
        qFatal("JSON error in database.json: %d", parseError.error);
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
}

DatabaseConnector::~DatabaseConnector()
{
    foreach (QString name, QSqlDatabase::connectionNames())
    {
        QSqlDatabase::removeDatabase(name);
        _databaseMutexes.remove(name);
    }
}

void DatabaseConnector::fetchWeekData(Queryable *entity, QString key)
{
    Q_UNUSED(key);
    QMutex *m = _databaseMutexes[entity->databaseName()];
    QFuture<Queryable::Context> future =
            QtConcurrent::run(executeWeekDataQuery, entity, key, m);
    QFutureWatcher<Queryable::Context> *watcher =
            new QFutureWatcher<Queryable::Context>();
    connect(watcher, SIGNAL(finished()), this, SLOT(processWeekDataQuery()));
    watcher->setFuture(future);
    emit fetchingStarted(entity);
}

void DatabaseConnector::processWeekDataQuery()
{
    QFutureWatcher<Queryable::Context> *watcher =
            reinterpret_cast<QFutureWatcher<Queryable::Context> *>(sender());
    watcher->deleteLater();

    Queryable::Context context = watcher->result();
    if (!context.isValid())
    {
        emit dataFetched(0, QList<NodeData *>());
        return;
    }

    QSqlQuery &query = context.query;
    QList<NodeData *> dataSet;
    while (query.next())
    {
        NodeData *data = new NodeData(query.value(0).toDateTime());
        uint i = 1;
        foreach (QString key, context.dataKeys)
        {
            bool ok = false;
            qreal value = query.value(i++).toReal(&ok);
            if (!ok)
                continue;
            data->setValue(key, value);
        }
        dataSet.append(data);
    }

    emit dataFetched(context.entity, dataSet);
}

void DatabaseConnector::fetchLatestData(Location *location)
{
    QMutex *m = _databaseMutexes[location->databaseAddress()];
    QFuture< QList<Queryable::Context> > future =
            QtConcurrent::run(executePoll, location, m);
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
        if (!context.isValid())
            continue;

        QSqlQuery &query = context.query;
        if (!query.next())      // We only want the first row
            continue;

        QDateTime dateTime = query.value(0).toDateTime();

        NodeLine *line = dynamic_cast<NodeLine *>(context.entity);
        if (line)
        {
            QList<double> temperatures;
            for (int i = 0; i < line->nodes().size(); i++)
            {
                bool ok = false;
                double temperature = query.value(i + 1).toDouble(&ok);
                if (!ok)
                    temperature = D_NO_DATA;
                temperatures.append(temperature);
            }
            emit dataPolled(line, temperatures, dateTime);
            continue;
        }

        Location *location = dynamic_cast<Location *>(context.entity);
        if (location)
        {
            QHash<QString, double> values;
            uint i = 1;
            foreach (QString key, context.dataKeys)
            {
                bool ok = false;
                double value = query.value(i++).toDouble(&ok);
                if (!ok)
                    value = D_NO_DATA;
                values.insert(key, value);
            }
            emit dataPolled(location, values, dateTime);
            continue;
        }
    }
}
