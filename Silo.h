/*****************************************************************************
 * Silo.h
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

#ifndef SILO_H
#define SILO_H

#include <QObject>
#include <QList>
#include <QString>
class Location;
class NodeLine;

class Silo : public QObject
{
    Q_OBJECT

public:
    explicit Silo(QObject *parent = 0);
    void addLine(NodeLine *line);

private:
    QString _name;
    Location *_location;
    QList<NodeLine *> _lines;

public:
    inline QString name() { return _name; }
    inline void setName(QString name) { _name = name; }
    inline Location *location() { return _location; }
    inline void setLocation(Location *location) { _location = location; }
    inline QList<NodeLine *> &lines() { return _lines; }
};

#endif // SILO_H
