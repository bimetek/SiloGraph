/*****************************************************************************
 * DatabaseConnector.h
 *
 * Created: 05/7 2013 by uranusjr
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

#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QObject>
#include <QDateTime>
#include <QHash>
#include <QList>
#include <QMutex>
class QSqlQuery;
class Node;
class NodeLine;
class Silo;
class Location;
class NodeData;

class DatabaseConnector : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseConnector(QObject *parent = 0);
    virtual ~DatabaseConnector();

public slots:
    void fetchWeekData(Node *node, Silo *silo);
    void fetchLatestData(Location *location);

signals:
    void fetchingStarted(Node *node, Silo *silo);
    void dataFetched(Node *node, Silo *silo, QList<NodeData *> dataSet);
    void dataPolled(NodeLine *line, QList<double> data, QDateTime dateTime);

private slots:
    void processWeekDataQuery();
    void processLatestDataQuery();

private:
    QHash<QString, QMutex *> _databaseMutexes;
};

#endif // DATABASECONNECTOR_H
