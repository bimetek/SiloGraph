/*****************************************************************************
 * Node.cpp
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

#include "Node.h"
#include <QDateTime>
#include <QMutexLocker>
#include <QVariant>
#include "NodeLine.h"
#include "Silo.h"
#include "Location.h"

Node::Node(QObject *parent) :
    NetworkElement(parent)
{
}

Queryable::Context Node::executeWeekDataFetch(QMutex *mutex)
{
    QMutexLocker locker(mutex);
    Q_UNUSED(locker);

    QSqlDatabase db = database();

    QString oneWeekAgo =
            QDateTime(QDate::currentDate().addDays(-7)).toString(Qt::ISODate);
    QString queryFormat =
            "SELECT date, %1 FROM rawdata "
            "WHERE silo_cable = :cableName AND date > :date ORDER BY date ASC";

    Queryable::Context context;
    context.entity = this;
    context.dataKeys = QStringList(name());

    context.query = QSqlQuery(db);
    context.query.prepare(queryFormat.arg(name()));
    context.query.bindValue(":cableName", QVariant(line()->name()));
    context.query.bindValue(":date", oneWeekAgo);
    context.query.exec();

    db.close();
    return context;
}

QSqlDatabase Node::database()
{
    QString dbn = line()->silo()->location()->databaseAddress();
    return QSqlDatabase::database(dbn);
}
