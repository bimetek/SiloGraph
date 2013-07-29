/*****************************************************************************
 * Queryable.h
 *
 * Created: 26/7 2013 by uranusjr
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

#ifndef QUERYABLE_H
#define QUERYABLE_H

#include <QSqlQuery>
#include <QStringList>
class QMutex;

class Queryable
{
public:
    struct Context
    {
        Context() : entity(0) {}

        QSqlQuery query;
        Queryable *entity;
        QStringList dataKeys;

        inline bool isValid()
        {
            if (!entity || !query.size())
                return false;
            return true;
        }
    };

    virtual QString databaseName() = 0;
    virtual QString nameForDataKey(const QString &key) { return key; }
    virtual QString unitForDataKey(const QString &) { return QString(); }
    virtual Context executeWeekDataFetch(
            QMutex *, QString = QString(), bool = true);
    virtual Context executePoll(QMutex *, QString, bool);
};

#endif // QUERYABLE_H
