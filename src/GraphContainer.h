/*****************************************************************************
 * GraphContainer.h
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
    void blockPlot();
    void unblockPlot();

protected:
    virtual void resizeEvent(QResizeEvent *);
    virtual QSize sizeHint() const;

private:
    QwtPlot *_plot;
    QwtPlotPanner *_panner;
    QwtPlotMagnifier *_magnifier;
    QwtPlotPicker *_picker;
    QwtScaleDiv _weekDiv;
};

#endif // GRAPHCONTAINER_H
