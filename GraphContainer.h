/*****************************************************************************
 * GraphContainer.h
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

#ifndef GRAPHCONTAINER_H
#define GRAPHCONTAINER_H

#include <QWidget>
#include <QList>
class QwtPlot;
class QwtPlotCurve;
class QwtPlotMagnifier;
class QwtPlotPanner;
class Node;
class Silo;
class NodeData;

class GraphContainer : public QWidget
{
    Q_OBJECT

public:
    explicit GraphContainer(QWidget *parent = 0);

public slots:
    void updatePlot(Node *node, Silo *silo,
                    QList <QList<NodeData *> > dataSets);

private:
    QwtPlot *_plot;
    QwtPlotPanner *_panner;
    QwtPlotMagnifier *_magnifier;
};

#endif // GRAPHCONTAINER_H
