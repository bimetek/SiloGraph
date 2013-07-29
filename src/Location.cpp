/*****************************************************************************
 * Location.cpp
 *
 * Created: 04/7 2013 by uranusjr
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

#include "Location.h"
#include <QDateTime>
#include <QMutexLocker>
#include <QVariant>
#include "Globals.h"
#include "Silo.h"

Location::Location(QObject *parent) :
    QObject(parent), Queryable()
{
}

Location::~Location()
{
}

void Location::addSilo(Silo *silo)
{
    silos().append(silo);
    silo->setLocation(this);
}

void Location::addSensor(const QString &key, const QString &value)
{
    sensorKeys().insert(key, value);
}

Queryable::Context Location::executePoll(QMutex *mutex, QString, bool close)
{
    QMutexLocker locker(mutex);
    Q_UNUSED(locker);

    QSqlDatabase db = QSqlDatabase::database(databaseName());

    QString queryFormat =
            "SELECT date, %1 FROM rawdata WHERE silo_cable = :cableName "
            "ORDER BY date DESC LIMIT 1";
    QStringList names(sensorKeys().keys());

    QString queryString = queryFormat.arg(names.join(", "));
    Queryable::Context context;
    context.entity = this;
    context.dataKeys = names;

    context.query = QSqlQuery(db);
    context.query.prepare(queryString);
    context.query.bindValue(":cableName", QVariant(sensorName()));
    context.query.exec();

    if (close)
        db.close();

    return context;
}

Queryable::Context Location::executeWeekDataFetch(QMutex *mutex,
                                                  QString key, bool close)
{
    if (key.isEmpty())
        return Queryable::Context();

    QMutexLocker locker(mutex);
    Q_UNUSED(locker);

    QSqlDatabase db = QSqlDatabase::database(databaseName());

    QString oneWeekAgo =
            QDateTime(QDate::currentDate().addDays(-7)).toString(Qt::ISODate);
    QString queryFormat =
            "SELECT date, %1 FROM rawdata "
            "WHERE silo_cable = :cableName AND date > :date ORDER BY date ASC";

    Queryable::Context context;
    context.entity = this;
    context.dataKeys = QStringList(key);

    context.query = QSqlQuery(db);
    context.query.prepare(queryFormat.arg(key));
    context.query.bindValue(":cableName", QVariant(sensorName()));
    context.query.bindValue(":date", oneWeekAgo);
    context.query.exec();

    if (close)
        db.close();

    return context;
}

QString Location::databaseName()
{
    return databaseAddress();
}

QString Location::nameForDataKey(const QString &key)
{
    return sensorKeys().value(key);
}

QString Location::unitForKey(const QString &key)
{
    QString name = sensorKeys().value(key);
    if (name.compare("temperature", Qt::CaseInsensitive) == 0)
        return DEGREE_SIGN;
    else if (name.compare("humidity", Qt::CaseInsensitive) == 0)
        return "%";
    return Queryable::unitForKey(key);
}
