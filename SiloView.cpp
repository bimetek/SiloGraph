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
#include "Globals.h"
#include "NodeLine.h"
#include "Node.h"
#include "Silo.h"
#include <QDebug>

SiloView::SiloView(Silo *silo, QWidget *parent) :
    QWidget(parent)
{
    _silo = silo;
    setObjectName(silo->name());

    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(QObject *)),
            this, SLOT(switchToNode(QObject *)));

    const int lineWidth = 30 * W_SCALE;
    const int lineMargin = 10 * W_SCALE;

    QPushButton *averageButton = new QPushButton(tr("Average"));
    connect(averageButton, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(averageButton, reinterpret_cast<Node *>(0));

    QHBoxLayout *siloLayout = new QHBoxLayout();
    siloLayout->setMargin(0);
    siloLayout->setContentsMargins(0, 0, 0, 0);
    foreach (NodeLine *line, silo->lines())
    {
        QWidget *lineView = new QWidget();
        lineView->setObjectName(line->name());

        QVBoxLayout *lineLayout = new QVBoxLayout();
        lineLayout->setMargin(0);
        lineLayout->setContentsMargins(0, 0, 0, 0);
        foreach (Node *node, line->nodes())
        {
            QPushButton *button = new QPushButton(node->name());
            button->setFixedSize(lineWidth, 15 * W_SCALE);
            button->setFont(defaultFontForSize(10 * W_SCALE));
            button->setObjectName(node->name());
            connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
            mapper->setMapping(button, node);
            lineLayout->addWidget(button);
        }
        lineView->setLayout(lineLayout);
        siloLayout->addWidget(lineView);
    }

    int lineCount = silo->lines().size();
    setFixedWidth(lineWidth * lineCount + lineMargin * (lineCount + 1));
    averageButton->setFixedWidth((lineWidth + lineMargin) * lineCount);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(averageButton);
    layout->addLayout(siloLayout);
    setLayout(layout);
}

void SiloView::switchToNode(QObject *obj)
{
    Node *node = reinterpret_cast<Node *>(obj);
    emit targetSwitched(node, _silo);
}

void SiloView::updateLatestData(NodeLine *line, QList<double> &data)
{
    QWidget *lineView = findChild<QWidget *>(line->name());
    QList<Node *> &nodes = line->nodes();
    for (int i = 0; i < data.size(); i++)
    {
        QPushButton *nodeButton =
                lineView->findChild<QPushButton *>(nodes[i]->name());
        if (nodeButton)
            nodeButton->setText(QString::number(data[i], 'f', 1));
        else
            qDebug() << "Node button" << nodes[i]->name() <<
                        "not found in line" << lineView->objectName() <<
                        "of silo" << objectName();
    }
}
