/*****************************************************************************
 * SiloListView.cpp
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

#include "SiloListView.h"
#include "Location.h"
#include "Silo.h"
#include "NodeLine.h"
#include "Node.h"
#include <QDebug>

SiloListView::SiloListView(QWidget *parent) :
    QWidget(parent)
{
}

void SiloListView::displaySilos(Location *location)
{
    foreach (Silo *silo, location->silos())
    {
        qDebug() << "Silo at" << location->name();
        foreach (NodeLine *line, silo->lines())
        {
            QString text;
            foreach (Node *node, line->nodes())
            {
                text.append(node->name());
                text.append(", ");
            }
            qDebug() << text;
        }
    }
}
