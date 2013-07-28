/*****************************************************************************
 * NodeLine.h
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

#ifndef NODELINE_H
#define NODELINE_H

#include <QObject>
#include "Queryable.h"
#include <QList>
class Silo;
class Node;

class NodeLine : public QObject, public Queryable
{
    Q_OBJECT

public:
    explicit NodeLine(QObject *parent = 0);
    void addNode(Node *node);
    virtual Context executePoll(QMutex *mutex,
                                QString = QString(), bool close = true);
    virtual QString databaseName();

private:
    QString _name;
    Silo *_silo;
    QList<Node *> _nodes;

public:
    inline QString name() { return _name; }
    inline void setName(QString name) { _name = name; }
    inline Silo *silo() { return _silo; }
    inline void setSilo(Silo *silo) { _silo = silo; }
    inline QList<Node *> &nodes() { return _nodes; }
};

#endif // NODELINE_H
