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
class QMutex;

class Queryable
{
public:
    struct Context
    {
        QSqlQuery query;
        Queryable *entity;
        uint dataCount;
    };

    virtual Context executeWeekDataFetch(QMutex *, bool close = true)
    {
        Q_UNUSED(close);
        return Context();
    }
    virtual Context executePoll(QMutex *, bool close = false)
    {
        Q_UNUSED(close);
        return Context();
    }

protected:
    virtual QSqlDatabase database() = 0;
};

#endif // QUERYABLE_H
