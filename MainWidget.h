/*****************************************************************************
 * MainWidget.h
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

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>

class GraphContainer;
class MapView;
class SiloListView;
class DatabaseConnector;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);

private:
    GraphContainer *_graphContainer;
    MapView *_mapView;
    SiloListView *_siloListView;
    DatabaseConnector *_dbc;
};

#endif // MAINWIDGET_H
