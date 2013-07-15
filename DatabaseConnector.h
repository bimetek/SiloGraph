/*****************************************************************************
 * DatabaseConnector.h
 *
 * Created: 05/7 2013 by uranusjr
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

#ifndef DATABASECONNECTOR_H
#define DATABASECONNECTOR_H

#include <QObject>
#include <QDateTime>
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

public slots:
    void fetchWeekData(Node *node, Silo *silo);
    void fetchLatestData(Location *location);

signals:
    void dataFetched(Node *node, Silo *silo,
                     QList <QList<NodeData *> > dataSets);
    void dataPolled(NodeLine *line, QList<double> data, QDateTime dateTime);

private slots:
    void processWeekDataQuery();
    void processLatestDataQuery();

private:
    QMutex _databaseMutex;
};

#endif // DATABASECONNECTOR_H
