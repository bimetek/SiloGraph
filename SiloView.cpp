/*****************************************************************************
 * SiloView.cpp
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

#include "SiloView.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QSignalMapper>
#include <QVBoxLayout>
#include "NodeLine.h"
#include "Node.h"
#include "Silo.h"
#include <QDebug>

SiloView::SiloView(Silo *silo, QWidget *parent) :
    QWidget(parent)
{
    _silo = silo;
    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(QObject *)),
            this, SLOT(switchToNode(QObject *)));
    QVBoxLayout *layout = new QVBoxLayout();
    QPushButton *averageButton = new QPushButton(tr("Average"));
    connect(averageButton, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(averageButton, 0);
    foreach (NodeLine *line, silo->lines())
    {
        QHBoxLayout *lineLayout = new QHBoxLayout();
        foreach (Node *node, line->nodes())
        {
            QPushButton *button = new QPushButton(node->name());
            connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
            mapper->setMapping(button, node);
            lineLayout->addWidget(button);
        }
        layout->addLayout(lineLayout);
    }
    setLayout(layout);
}

void SiloView::switchToNode(QObject *obj)
{
    Node *node = reinterpret_cast<Node *>(obj);
    emit targetSwitched(node, _silo);
}
