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
#include "Location.h"
#include "Silo.h"
#include "Node.h"
#include <QDebug>

DatabaseConnector::DatabaseConnector(QObject *parent) :
    QObject(parent)
{
}

void DatabaseConnector::fetchData(Node *node, Silo *silo)
{
    qDebug() << "Fetching data for" << node->name()
             << "in silo" << silo->name()
             << "for" << silo->location()->name();
}
