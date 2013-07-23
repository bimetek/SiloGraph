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
#include <QSize>
#include <qwt_scale_div.h>
class QwtPlot;
class QwtPlotCurve;
class QwtPlotMagnifier;
class QwtPlotPanner;
class QwtPlotPicker;
class Node;
class Silo;
class NodeData;

class GraphContainer : public QWidget
{
    Q_OBJECT

public:
    explicit GraphContainer(QWidget *parent = 0);

public slots:
    void clearPlot();               // Default replot = true
    void clearPlot(bool replot);
    void updatePlot(Node *node, Silo *silo,
                    QList <QList<NodeData *> > dataSets);

protected:
    virtual QSize sizeHint() const;

private:
    QwtPlot *_plot;
    QwtPlotPanner *_panner;
    QwtPlotMagnifier *_magnifier;
    QwtPlotPicker *_picker;
    QwtScaleDiv _weekDiv;
};

#endif // GRAPHCONTAINER_H
