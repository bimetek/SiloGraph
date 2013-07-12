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
        return getCurrentLocale().toString(dt, "HH:mm:ss");
    }
    void setDateTime(QDateTime dt) { _datetime = dt; }

private:
    QDateTime _datetime;
};


GraphContainer::GraphContainer(QWidget *parent) :
    QWidget(parent)
{
    _plot = new QwtPlot();
    _plot->setAxisScaleDraw(QwtPlot::xBottom,
                            new DateTimeDraw(QDateTime::currentDateTime()));

    // Enable float-ending to make curve "fit" to plot's right
    QwtScaleEngine *engine = _plot->axisScaleEngine(QwtPlot::xBottom);
    engine->setAttribute(QwtScaleEngine::Floating, true);

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->addWidget(_plot, 0, 0);
    setLayout(mainLayout);
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
        QDateTime dateTime = dataSet.first()->dateTime();
        if (dateTime < firstDateTime)
            firstDateTime = dateTime;
    }
    foreach (QList<NodeData *> dataSet, dataSets)
    {
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
        curve->attach(_plot);
        curve->setSamples(points);
    }

    _plot->setAxisScaleDraw(QwtPlot::xBottom,
                            new DateTimeDraw(firstDateTime));
    _plot->replot();
}
