/*****************************************************************************
 * Silo.h
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

#ifndef SILO_H
#define SILO_H

#include <QObject>
#include "Queryable.h"
#include <QList>
#include <QString>
class Location;
class NodeLine;

class Silo : public QObject, public Queryable
{
    Q_OBJECT

public:
    explicit Silo(QObject *parent = 0);
    void addLine(NodeLine *line);
    virtual Queryable::Context executeWeekDataFetch(QMutex *mutex);

protected:
    virtual QSqlDatabase database();

private:
    QString _name;
    Location *_location;
    QList<NodeLine *> _lines;

public:
    inline QString name() const { return _name; }
    inline void setName(QString name) { _name = name; }
    inline Location *location() const { return _location; }
    inline void setLocation(Location *location) { _location = location; }
    inline QList<NodeLine *> &lines() { return _lines; }
};

#endif // SILO_H
