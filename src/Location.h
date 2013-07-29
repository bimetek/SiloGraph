/*****************************************************************************
 * Location.h
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

#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>
#include "Queryable.h"
#include <QList>
#include <QHash>
#include <QSqlDatabase>
class Silo;

class Location : public QObject, public Queryable
{
    Q_OBJECT

public:
    explicit Location(QObject *parent = 0);
    virtual ~Location();
    void addSilo(Silo *silo);
    void addSensor(const QString &key, const QString &value);
    virtual Context executePoll(QMutex *mutex,
                                QString = QString(), bool close = true);
    virtual Context executeWeekDataFetch(QMutex *mutex, QString key,
                                         bool close = true);
    virtual QString databaseName();
    virtual QString nameForDataKey(const QString &key);
    virtual QString unitForDataKey(const QString &key);

private:
    QString _name;
    QString _databaseAddress;
    double _latitude;
    double _longitude;
    QList<Silo *> _silos;
    QString _sensorName;
    QHash<QString, QString> _sensorKeys;

public:
    inline QString name() { return _name; }
    inline void setName(QString name) { _name = name; }
    inline QString databaseAddress() { return _databaseAddress; }
    inline void setDatabaseAddress(QString addr) { _databaseAddress = addr; }
    inline double latitude() { return _latitude; }
    inline void setLatitude(double latitude) { _latitude = latitude; }
    inline double longitude() { return _longitude; }
    inline void setLongitude(double longitude) { _longitude = longitude; }
    inline QList<Silo *> &silos() { return _silos; }
    QString sensorName() const { return _sensorName; }
    void setSensorName(const QString &sensorName) { _sensorName = sensorName; }
    QHash<QString, QString> &sensorKeys() { return _sensorKeys; }
};

#endif // LOCATION_H
