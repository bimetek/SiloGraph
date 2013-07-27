/*****************************************************************************
 * NodeLine.cpp
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

#include "NodeLine.h"
#include <QMutexLocker>
#include <QStringList>
#include <QVariant>
#include "Node.h"
#include "Silo.h"
#include "Location.h"

NodeLine::NodeLine(QObject *parent) :
    NetworkElement(parent)
{
}

void NodeLine::addNode(Node *node)
{
    nodes().append(node);
    node->setLine(this);
}

Queryable::Context NodeLine::executePoll(QMutex *mutex, bool close)
{
    QMutexLocker locker(mutex);
    Q_UNUSED(locker);

    QSqlDatabase db = QSqlDatabase::database(databaseName());

    QString queryFormat =
            "SELECT date, %1 FROM rawdata WHERE silo_cable = :cableName "
            "ORDER BY date DESC LIMIT 1";
    QStringList names;
    foreach (Node *node, nodes())
        names << node->name();
    QString queryString = queryFormat.arg(names.join(", "));
    Queryable::Context context;
    context.entity = this;
    context.dataKeys = QStringList(name());

    context.query = QSqlQuery(db);
    context.query.prepare(queryString);
    context.query.bindValue(":cableName", QVariant(name()));
    context.query.exec();

    if (close)
        db.close();

    return context;
}

QString NodeLine::databaseName()
{
    return silo()->location()->databaseAddress();
}
