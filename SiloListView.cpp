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
#include <QHBoxLayout>
#include <QLabel>
#include <QTimerEvent>
#include "Globals.h"
#include "Location.h"
#include "Silo.h"
#include "NodeLine.h"
#include "Node.h"
#include "SiloView.h"
#include <QDebug>

SiloListView::SiloListView(QWidget *parent) :
    QWidget(parent),
    _currentLocation(0),
    _pollingTimerId(0)
{
    _lastUpdate = new QLabel();
    _siloListLayout = new QHBoxLayout();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addStretch(1);
    topLayout->addWidget(_lastUpdate);
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(_siloListLayout);
    setLayout(mainLayout);
}

void SiloListView::setLocation(Location *location)
{
    if (_currentLocation == location)
        return;

    _lastUpdate->setText("");
    _currentLocation = location;

    clearLayout(_siloListLayout);
    foreach (Silo *silo, location->silos())
    {
        SiloView *sv = new SiloView(silo);
        _siloListLayout->addWidget(sv);
        connect(sv, SIGNAL(targetSwitched(Node *,Silo *)),
                this, SIGNAL(targetSwitched(Node *,Silo *)));
    }

    if (_pollingTimerId)
        killTimer(_pollingTimerId);
    _pollingTimerId = startTimer(5 * 60 * 1000);    // ms

    if (_currentLocation)
        emit shouldPollForLocation(_currentLocation);   // Poll now
}

void SiloListView::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == _pollingTimerId)
    {
        if (_currentLocation)
            emit shouldPollForLocation(_currentLocation);
    }
}

void SiloListView::updateLatestData(NodeLine *line, QList<double> data,
                                    QDateTime dateTime)
{
    Silo *silo = line->silo();
    SiloView *sv = findChild<SiloView *>(silo->name());
    sv->updateLatestData(line, data);
    refreshLastUpdate(dateTime);
}

void SiloListView::refreshLastUpdate(QDateTime &dateTime)
{
    QString dts = getCurrentLocale().toString(dateTime, "yyyy-MM-dd HH:mm:ss");
    _lastUpdate->setText(QString("Last update: %1").arg(dts));
}
