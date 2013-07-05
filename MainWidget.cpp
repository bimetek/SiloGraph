/*****************************************************************************
 * MainWidget.cpp
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

#include "MainWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "DatabaseConnector.h"
#include "GraphContainer.h"
#include "MapView.h"
#include "SiloListView.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    _dbc = new DatabaseConnector(this);
    _mapView = new MapView();
    _siloListView = new SiloListView();
    _plotContainer = new GraphContainer();

    // Right side
    QVBoxLayout *detailLayout = new QVBoxLayout();
    detailLayout->addWidget(_siloListView, 1);
    detailLayout->addWidget(_plotContainer, 1);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(_mapView, 0);
    mainLayout->addLayout(detailLayout, 1);
    setLayout(mainLayout);

    connect(_mapView, SIGNAL(locationChanged(Location *)),
            _siloListView, SLOT(setLocation(Location *)));
    connect(_siloListView, SIGNAL(targetSwitched(Node *,Silo *)),
            _dbc, SLOT(fetchData(Node *,Silo *)));
    connect(_dbc, SIGNAL(dataFetched(Node *, Silo *, QList<NodeData *>)),
            _plotContainer,
            SLOT(updatePlot(Node *, Silo *, QList<NodeData *>)));
}
