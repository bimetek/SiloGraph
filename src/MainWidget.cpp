/*****************************************************************************
 * MainWidget.cpp
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

#include "MainWidget.h"
#include <QHBoxLayout>
#include <QString>
#include <QTimerEvent>
#include <QVBoxLayout>
#include "DatabaseConnector.h"
#include "GraphContainer.h"
#include "MapContainer.h"
#include "SiloListView.h"
#include "LogoHolder.h"
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    _dbc = new DatabaseConnector(this);
    _mapContainer = new MapContainer();
    _siloListView = new SiloListView();
    _plotContainer = new GraphContainer();

    // Right side
    QVBoxLayout *detailLayout = new QVBoxLayout();
    detailLayout->setMargin(0);
    detailLayout->addWidget(_siloListView);
    detailLayout->addWidget(_plotContainer);

    // Left side
    QVBoxLayout *overviewLayout = new QVBoxLayout();
    overviewLayout->addWidget(_mapContainer, 1);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(overviewLayout, 1);
    mainLayout->addLayout(detailLayout, 4);
    setLayout(mainLayout);

    connect(_mapContainer, SIGNAL(locationChanged(Location *)),
            _siloListView, SLOT(setLocation(Location *)));
    connect(_mapContainer, SIGNAL(locationChanged(Location *)),
            _plotContainer, SLOT(clearPlot()));
    connect(_siloListView, SIGNAL(targetSwitched(Queryable *, QString)),
            _dbc, SLOT(fetchWeekData(Queryable *, QString)));
    connect(_siloListView, SIGNAL(shouldPollForLocation(Location *)),
            _dbc, SLOT(fetchLatestData(Location *)));
    connect(_dbc, SIGNAL(dataFetched(Node *, Silo *, QList<NodeData *>)),
            _plotContainer, SLOT(updatePlot(
                                     Node *, Silo *, QList<NodeData *>)));
    connect(_dbc, SIGNAL(dataPolled(NodeLine *, QList<double>, QDateTime)),
            _siloListView, SLOT(updateLatestData(
                                    NodeLine *, QList<double>, QDateTime)));
    connect(_dbc, SIGNAL(dataPolled(
                             Location *, QHash<QString,double>, QDateTime)),
            _siloListView, SLOT(updateLatestData(
                            Location *, QHash<QString, double>, QDateTime)));
    connect(_dbc, SIGNAL(fetchingStarted(Queryable *)),
            _plotContainer, SLOT(blockPlot()));
}
