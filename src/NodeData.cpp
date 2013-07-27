/*****************************************************************************
 * NodeData.cpp
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

#include "NodeData.h"
#include <QDebug>

NodeData::NodeData(const QDateTime &dateTime, QObject *parent) :
    QObject(parent)
{
    setDateTime(dateTime);
}

int NodeData::valueCount() const
{
    return _storage.size();
}

QList<QString> NodeData::keys() const
{
    return _storage.keys();
}

qreal NodeData::value(const QString &key) const
{
    return _storage.value(key);
}

qreal NodeData::value(const QString &key, const qreal &defaultValue) const
{
    return _storage.value(key, defaultValue);
}

void NodeData::setValue(const QString &key, const qreal &value)
{
    _storage.insert(key, value);
}

