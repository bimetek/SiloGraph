/*****************************************************************************
 * SiloView.cpp
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

#include "SiloView.h"
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QSignalMapper>
#include <QStyleOption>
#include <QVBoxLayout>
#include "Globals.h"
#include "NodeLine.h"
#include "Node.h"
#include "Silo.h"
#include <QDebug>

SiloView::SiloView(Silo *silo, QWidget *parent) :
    QWidget(parent)
{
    // Data initialization
    _silo = silo;
    setObjectName(silo->name());

    // Misc. UI initialization
    _lastUpdate = new QLabel();
    _lastUpdate->setAlignment(Qt::AlignHCenter);
    _lastUpdate->setStyleSheet("font-size: 11pt;");
    _backgroundHolder = new QLabel(this);
    QString styleSheet =
        "border-image: url(:/img/silo_background.png) 0 0 0 0 stretch stretch;";
    _backgroundHolder->setStyleSheet(styleSheet);

    // Setup signal mapper for silo buttons
    QSignalMapper *mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(QObject *)),
            this, SLOT(switchToObject(QObject *)));

    // Silo average button
    QString buttonStyle = textFromFile(":/assets/silo_button_styles.css");
    QString averageButtonTitle =
            silo->name().replace('s', "Silo ", Qt::CaseInsensitive);
    QPushButton *averageButton = new QPushButton(averageButtonTitle);
    averageButton->setObjectName("averageButton");
    averageButton->setStyleSheet(buttonStyle + "padding: 0 0.5em;");
    connect(averageButton, SIGNAL(clicked()), mapper, SLOT(map()));
    mapper->setMapping(averageButton, _silo);

    // Silo cables' buttons; also lay them out at the same time
    QHBoxLayout *siloLayout = new QHBoxLayout();
    siloLayout->setContentsMargins(0, 0, 0, 0);
    siloLayout->addStretch(1);
    foreach (NodeLine *line, silo->lines())
    {
        QWidget *lineView = new QWidget();
        lineView->setObjectName(line->name());

        QVBoxLayout *lineLayout = new QVBoxLayout();
        lineLayout->setContentsMargins(0, 0, 0, 0);
        lineLayout->setAlignment(Qt::AlignBottom);
        foreach (Node *node, line->nodes())
        {
            QString nodeButtonTitle =
                    node->name().replace('s', "Node ", Qt::CaseInsensitive);
            QPushButton *button = new QPushButton(nodeButtonTitle);
            button->setObjectName(node->name());
            button->setStyleSheet(buttonStyle);
            connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
            mapper->setMapping(button, node);

            // Add buttons from bottom to top
            lineLayout->insertWidget(0, button);
        }
        lineView->setLayout(lineLayout);

        siloLayout->addWidget(lineView, 1);
    }
    siloLayout->addStretch(1);

    // Lay out the whole silo
    QHBoxLayout *averageLayout = new QHBoxLayout();
    averageLayout->addStretch(1);
    averageLayout->addWidget(averageButton);
    averageLayout->addStretch(1);

    QVBoxLayout *buttonsLayout = new QVBoxLayout();
    buttonsLayout->setContentsMargins(0, 0, 0, 0);
    buttonsLayout->addLayout(averageLayout);
    buttonsLayout->addLayout(siloLayout);

    // Lay out the silo with silo info UI
    QVBoxLayout *siloInfoLayout = new QVBoxLayout();
    siloInfoLayout->addSpacing(1);
    siloInfoLayout->addLayout(buttonsLayout);
    siloInfoLayout->addStretch(5);
    siloInfoLayout->addWidget(_lastUpdate);

    // Add paddings to left and right of silo content
    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addStretch(1);
    mainLayout->addLayout(siloInfoLayout);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

void SiloView::resizeEvent(QResizeEvent *)
{
    // Recalculate the background holder's geometry. The new height should fill
    // the parent, width calculated by keeping aspect. Left and right margins
    // are so that the holder is horizontally centered.
    int h = size().height() - 25;   // Minus height for _lastUpdate
    int w = (_silo->lines().size() + 1) * 70;
    int left = (size().width() - w) / 2;
    _backgroundHolder->setGeometry(left, 0, w, h);
}

void SiloView::switchToObject(QObject *obj)
{
    Queryable *entity = dynamic_cast<Queryable *>(obj);
    if (!entity)
        return;
    emit targetSwitched(entity);
}

void SiloView::updateLatestData(NodeLine *line, QList<double> &data,
                                QDateTime dateTime)
{
    QWidget *lineView = findChild<QWidget *>(line->name());
    QList<Node *> &nodes = line->nodes();
    for (int i = 0; i < data.size(); i++)
    {
        QString name = nodes[i]->name();
        QPushButton *nodeButton = lineView->findChild<QPushButton *>(name);
        if (nodeButton)
        {
            QString newText;
            if (data[i] != D_NO_DATA)
            {
                QString number = QString::number(data[i], 'f', 1);
                newText = QString("%1%2").arg(number, DEGREE_SIGN);
            }
            else
            {
                newText = "--";
            }
            nodeButton->setText(newText);
        }
        else
        {
            qDebug() << "Node button" << nodes[i]->name() <<
                        "not found in line" << lineView->objectName() <<
                        "of silo" << objectName();
        }
    }
    refreshLastUpdate(dateTime);
}

void SiloView::refreshLastUpdate(QDateTime &dateTime)
{
    if (dateTime.isValid())
        refreshLastUpdate(dateTime.toString("yyyy-MM-dd HH:mm:ss"));
    else
        refreshLastUpdate("--");
}

void SiloView::refreshLastUpdate(QString text, bool clearAll)
{
    if (clearAll)
        _lastUpdate->setText(text);
    else
        _lastUpdate->setText(QString("Last update: %1").arg(text));
}

void SiloView::invalidateLastUpdate()
{
    refreshLastUpdate("Loading...", true);
}
