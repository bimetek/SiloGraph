/*****************************************************************************
 * Node.h
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

#ifndef NODE_H
#define NODE_H

#include <QObject>

class Node : public QObject
{
    Q_OBJECT

public:
    explicit Node(QObject *parent = 0);

private:
    QString _name;

public:
    inline QString name() { return _name; }
    inline void setName(QString name) { _name = name; }
};

#endif // NODE_H
