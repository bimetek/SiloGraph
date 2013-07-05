/*****************************************************************************
 * DatabaseConnector.cpp
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

#include "DatabaseConnector.h"
#include <QFile>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariantMap>
#include <qjson/parser.h>
#include "Location.h"
#include "Silo.h"
#include "NodeLine.h"
#include "Node.h"
#include <QDebug>

DatabaseConnector::DatabaseConnector(QObject *parent) :
    QObject(parent)
{
    QJson::Parser parser;
    bool success = false;
    QFile *dbFile = new QFile(":/data/databases.json");
    QVariant result = parser.parse(dbFile, &success);
    delete dbFile;

    if (!success)
    {
        qFatal("Database JSON cannot be parsed");
        return;
    }
    QVariantMap dbs = result.toMap()["databases"].toMap();
    foreach (QString key, dbs.keys())
    {
        QVariantMap info = dbs[key].toMap();
        QString address = info["address"].toString();
        QSqlDatabase db = QSqlDatabase::addDatabase(info["driver"].toString(),
                                                    address);
        db.setHostName(address);
        db.setUserName(info["username"].toString());
        db.setPassword(info["password"].toString());
        db.setDatabaseName("almin");
        if (info.contains("port"))
            db.setPort(info["port"].toInt());
    }
}

void DatabaseConnector::fetchData(Node *node, Silo *silo)
{
    QString address = silo->location()->databaseAddress();
    QSqlDatabase db = QSqlDatabase::database(address);
    if (node)
    {
        QString queryFormat =
                "SELECT %1, date FROM rawdata WHERE silo_cable=\"%2\" "
                "ORDER BY date DESC LIMIT 10";
        QSqlQuery query = db.exec(queryFormat.arg(node->name(),
                                                  node->line()->name()));
        if (query.size())
        {
            QString text = "";
            while (query.next())
            {
                for (int i = 0; i < query.record().count(); i++)
                {
                    QString value = query.value(i).toString();
                    text += (value + " ");
                }
                text += "\n";
            }
            qDebug() << text;
        }
    }
}
