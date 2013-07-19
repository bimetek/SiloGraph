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
    _siloListLayout = new QHBoxLayout();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(_siloListLayout, 1);
    setLayout(mainLayout);

    setMinimumHeight(340);
}

void SiloListView::setLocation(Location *location)
{
    if (_currentLocation == location)
        return;

    _currentLocation = location;

    clearLayout(_siloListLayout);
    foreach (Silo *silo, location->silos())
    {
        SiloView *sv = new SiloView(silo);
        _siloListLayout->addWidget(sv);
        connect(sv, SIGNAL(targetSwitched(Node *,Silo *)),
                this, SIGNAL(targetSwitched(Node *,Silo *)));
        connect(this, SIGNAL(shouldPollForLocation(Location *)),
                sv, SLOT(invalidateLastUpdate()));
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
    // If the fetched data is out of date and is from some previous location,
    // ignore it.
    if (_currentLocation != line->silo()->location())
        return;

    Silo *silo = line->silo();
    SiloView *sv = findChild<SiloView *>(silo->name());
    sv->updateLatestData(line, data, dateTime);
}
