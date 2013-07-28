/*****************************************************************************
 * Node.h
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

#ifndef NODE_H
#define NODE_H

#include <QObject>
#include "Queryable.h"
class QMutex;
class Node;
class NodeLine;
class Silo;

class Node : public QObject, public Queryable
{
    Q_OBJECT

public:
    explicit Node(QObject *parent = 0);
    virtual ~Node();
    virtual Queryable::Context executeWeekDataFetch(
            QMutex *mutex, QString = QString(), bool close = true);
    virtual QString databaseName();

private:
    QString _name;
    NodeLine *_line;

public:
    inline QString name() { return _name; }
    inline void setName(QString name) { _name = name; }
    inline NodeLine *line() { return _line; }
    inline void setLine(NodeLine *line) { _line = line; }
};

#endif // NODE_H
