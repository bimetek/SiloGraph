/*****************************************************************************
 * Silo.cpp
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

#include "Silo.h"
#include <QDateTime>
#include <QMutexLocker>
#include <QVariant>
#include "NodeLine.h"
#include "Location.h"

Silo::Silo(QObject *parent) :
    QObject(parent)
{
}

 void Silo::addLine(NodeLine *line)
 {
     lines().append(line);
     line->setSilo(this);
 }

 Queryable::Context Silo::executeWeekDataFetch(QMutex *mutex)
 {
     QMutexLocker locker(mutex);
     Q_UNUSED(locker);

     QSqlDatabase db = database();

     QString oneWeekAgo =
             QDateTime(QDate::currentDate().addDays(-7)).toString(Qt::ISODate);
     QString queryFormat =
             "SELECT date, Full, Empty FROM average "
             "WHERE silo = :siloName AND date > :date ORDER BY date ASC";

     Queryable::Context context;
     context.entity = this;

     QStringList keys;
     keys << "Full" << "Empty";
     context.dataKeys = keys;

     context.query = QSqlQuery(db);
     context.query.prepare(queryFormat);
     context.query.bindValue(":siloName", QVariant(name()));
     context.query.bindValue(":date", oneWeekAgo);
     context.query.exec();

     db.close();
     return context;
 }

 QSqlDatabase Silo::database()
 {
     QString dbn = location()->databaseAddress();
     return QSqlDatabase::database(dbn);
 }
