/*****************************************************************************
 * NodeData.h
 *
 * Created: 06/7 2013 by uranusjr
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

#ifndef NODEDATA_H
#define NODEDATA_H

#include <QObject>
#include <QDateTime>

class NodeData : public QObject
{
    Q_OBJECT

public:
    explicit NodeData(QObject *parent = 0);

private:
    QDateTime _dateTime;
    double _temperature;

public:
    inline QDateTime dateTime() { return _dateTime; }
    inline void setDateTime(QDateTime dateTime) { _dateTime = dateTime; }
    inline double temperature() { return _temperature; }
    inline void setTemperature(double temp) { _temperature = temp; }
};

#endif // NODEDATA_H
