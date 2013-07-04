/*****************************************************************************
 * NodeLine.h
 *
 * Created: 04/7 2013 by uranusjr
 *
 * Copyright 2013 uranusjr. All rights reserved.
 *
 * This file is published under the Creative Commons 3.0.
 * http://creativecommons.org/licenses/by/3.0/
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/

#ifndef NODELINE_H
#define NODELINE_H

#include <QObject>
#include <QList>
class Node;

class NodeLine : public QObject
{
    Q_OBJECT

public:
    explicit NodeLine(QObject *parent = 0);
    inline void addNode(Node *node) { nodes().append(node); }

private:
    QList<Node *> _nodes;

public:
    inline QList<Node *> &nodes() { return _nodes; }
};

#endif // NODELINE_H
