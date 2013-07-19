/*****************************************************************************
 * GraphContainer.cpp
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

#include "GraphContainer.h"
#include <QDateTime>
#include <QGridLayout>
#include <QPointF>
#include <QVector>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_engine.h>
#include "Globals.h"
#include "Node.h"
#include "Silo.h"
#include "NodeData.h"
#include <QDebug>


class DateTimeDraw : public QwtScaleDraw
{
public:
    DateTimeDraw(QDateTime dt) { setDateTime(dt); }
    virtual QwtText label(double v) const
    {
        QDateTime dt = _datetime.addSecs(v);
        return getCurrentLocale().toString(dt, "yyyy-MM-dd\n   HH:mm:ss");
    }
    void setDateTime(QDateTime dt) { _datetime = dt; }

private:
    QDateTime _datetime;
};

class LimitedPanner : public QwtPlotPanner
{
public:
    LimitedPanner(QWidget *canvas) : QwtPlotPanner(canvas)
    {
        QwtScaleMap xBottom = plot()->canvasMap(QwtPlot::xBottom);
        QwtScaleMap yLeft = plot()->canvasMap(QwtPlot::yLeft);

        _initials.left = xBottom.s1();
        _initials.right = xBottom.s2();
        _initials.bottom = yLeft.s1();
        _initials.top = yLeft.s2();

        _transformed.left = xBottom.p1();
        _transformed.right = xBottom.p2();
        _transformed.bottom = yLeft.p1();
        _transformed.top = yLeft.p2();
    }

protected:
    virtual void moveCanvas(int dx, int dy)
    {
        double oldLeft = plot()->transform(QwtPlot::xBottom, _initials.left);
        double oldRight = plot()->transform(QwtPlot::xBottom, _initials.right);
        double oldBottom = plot()->transform(QwtPlot::yLeft, _initials.bottom);
        double oldTop = plot()->transform(QwtPlot::yLeft, _initials.top);

        if (oldLeft + dx > _transformed.left)       // Left overflow
            dx = _transformed.left - oldLeft;
        if (oldRight + dx < _transformed.right)     // Right overflow
            dx = _transformed.right - oldRight;
        if (oldBottom + dy < _transformed.bottom)   // Bottom overflow
            dy = _transformed.bottom - oldBottom;
        if (oldTop + dy > _transformed.top)         // Top overflow
            dy = _transformed.top - oldTop;

        return QwtPlotPanner::moveCanvas(dx, dy);
    }

private:
    struct {
        double left;
        double right;
        double top;
        double bottom;
    } _initials;
    struct {
        double left;
        double right;
        double top;
        double bottom;
    } _transformed;
};


class LimitedMagnifier : public QwtPlotMagnifier
{
public:
    LimitedMagnifier(QWidget *canvas)
        : QwtPlotMagnifier(canvas), _zoom(1.0)
    {
        setWheelFactor(1.0 / wheelFactor());    // Inverse scrolling direction
    }

protected:
    void rescale(double factor)
    {
        double nextZoom = factor * _zoom;
        if (nextZoom > 1.0)   // Overly zoomed out
        {
            nextZoom = 1.0;
            factor = 1.0 / _zoom;
        }
        _zoom = nextZoom;
        QwtPlotMagnifier::rescale(factor);
    }

private:
    double _zoom;
};


GraphContainer::GraphContainer(QWidget *parent) :
    QWidget(parent), _panner(0), _magnifier(0)
{
    _plot = new QwtPlot();
    _plot->setAxisScaleDraw(QwtPlot::xBottom,
                            new DateTimeDraw(QDateTime::currentDateTime()));

    // Enable float-ending to make curve "fit" to plot's right
    QwtScaleEngine *engine = _plot->axisScaleEngine(QwtPlot::xBottom);
    engine->setAttribute(QwtScaleEngine::Floating, true);

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(_plot, 0, 0);
    setLayout(mainLayout);

    setMinimumSize(200, 150);
    _plot->setMinimumSize(200, 150);
}

void GraphContainer::updatePlot(Node *node, Silo *silo,
                                QList <QList<NodeData *> > dataSets)
{
    Q_UNUSED(node);
    Q_UNUSED(silo);

    _plot->detachItems();   // Clear old drawings

    // Find the earliest date time as x axis's zero
    QDateTime firstDateTime = QDateTime::currentDateTime();
    foreach (QList<NodeData *> dataSet, dataSets)
    {
        if (dataSet.isEmpty())
            continue;
        QDateTime dateTime = dataSet.first()->dateTime();
        if (dateTime < firstDateTime)
            firstDateTime = dateTime;
    }
    int i = 0;
    double dataCount = dataSets.size();
    foreach (QList<NodeData *> dataSet, dataSets)
    {
        if (dataSet.isEmpty())
            continue;

        QVector<QPointF> points;
        foreach (NodeData *data, dataSet)
        {
            QPointF point(firstDateTime.secsTo(data->dateTime()),
                          data->temperature());
            points.append(point);
        }
        qDeleteAll(dataSet);

        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setYAxis(QwtPlot::yLeft);
        QPen pen(QColor::fromHsvF(i++ / dataCount, 1.0, 1.0));
        pen.setWidth(2);
        curve->setPen(pen);
        curve->attach(_plot);
        curve->setSamples(points);
    }

    _plot->setAxisScaleDraw(QwtPlot::xBottom, new DateTimeDraw(firstDateTime));
    _plot->setAxisAutoScale(QwtPlot::xBottom);
    _plot->setAxisAutoScale(QwtPlot::yLeft);
    _plot->replot();

    if (_panner)
        _panner->deleteLater();
    _panner = new LimitedPanner(_plot->canvas());
    if (_magnifier)
        _magnifier->deleteLater();
    _magnifier = new LimitedMagnifier(_plot->canvas());
}
