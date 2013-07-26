/*****************************************************************************
 * NodeData.h
 *
 * Created: 06/7 2013 by uranusjr
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

#ifndef NODEDATA_H
#define NODEDATA_H

#include <QObject>
#include <QDateTime>
#include <QHash>
#include <QList>

class NodeData : public QObject
{
    Q_OBJECT

public:
    explicit NodeData(const QDateTime &dateTime = QDateTime::currentDateTime(),
                      QObject *parent = 0);
    int valueCount() const;
    QList<QString> keys() const;
    qreal value(const QString &key) const;
    qreal value(const QString &key, const qreal &defaultValue) const;
    void setValue(const QString &key, const qreal &value);

private:
    QDateTime _dateTime;
    QHash<QString, qreal> _storage;

public:
    inline QDateTime dateTime() const { return _dateTime; }
    inline void setDateTime(const QDateTime &dateTime) { _dateTime = dateTime; }
};

#endif // NODEDATA_H
