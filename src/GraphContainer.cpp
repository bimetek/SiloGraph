/*****************************************************************************
 * GraphContainer.cpp
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

#include "GraphContainer.h"
#include <QDateTime>
#include <QGridLayout>
#include <QPointF>
#include <QResizeEvent>
#include <QVector>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_div.h>
#include <qwt_scale_draw.h>
#include "Globals.h"
#include "Node.h"
#include "NodeLine.h"
#include "Silo.h"
#include "Location.h"
#include "NodeData.h"
#include "ProgressLayer.h"
#include "CurveTrackingPicker.h"
#include <QDebug>


class DateTimeDraw : public QwtScaleDraw
{
public:
    DateTimeDraw(QDateTime dt) : QwtScaleDraw() { setDateTime(dt); }
    virtual QwtText label(double v) const
    {
        return _datetime.addSecs(v).toString("yyyy-MM-dd\n   HH:mm:ss");
    }
    void setDateTime(QDateTime dt) { _datetime = dt; }

private:
    QDateTime _datetime;
};


GraphContainer::GraphContainer(QWidget *parent) :
    QWidget(parent), _panner(0), _magnifier(0), _picker(0)
{
    _plot = new QwtPlot();

    QDateTime oneWeekAgo(QDate::currentDate().addDays(-6));
    _plot->setAxisScaleDraw(QwtPlot::xBottom,
                            new DateTimeDraw(oneWeekAgo));

    double oneHour = 60 * 60 * 1.0;     // 1 hour in seconds
    QList<double> majorTicks;
    QList<double> mediumTicks;
    QList<double> minorTicks;
    for (int i = 0; i <= 7 * 24; i++)
    {
        double current = i * oneHour;
        if (i % 24 == 0)        // Full day
            majorTicks << current;
        else if (i % 12 == 0)   // Half day
            mediumTicks << current;
        else if (i % 6 == 0)    // Quarter dour
            minorTicks << current;
    }
    _weekDiv = QwtScaleDiv(0.0, 7 * 24 * oneHour,
                           minorTicks, mediumTicks, majorTicks);
    _plot->setAxisScaleDiv(QwtPlot::xBottom, _weekDiv);
    _plot->setAxisScaleDiv(QwtPlot::yLeft, QwtScaleDiv(10.0, 40.0));
    _picker = new CurveTrackingPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::NoRubberBand, QwtPicker::AlwaysOn,
                _plot->canvas(), oneWeekAgo);

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(_plot, 0, 0);
    setLayout(mainLayout);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMinimumSize(200, 150);
    _plot->setMinimumSize(200, 150);
}

void GraphContainer::resizeEvent(QResizeEvent *e)
{
    QList<ProgressLayer *> layers = findChildren<ProgressLayer *>();
    foreach (ProgressLayer *layer, layers)
        layer->resize(e->size());
    e->accept();
}

QSize GraphContainer::sizeHint() const
{
    return QSize(200, 150);
}

void GraphContainer::clearPlot()
{
    clearPlot(true);
}

void GraphContainer::clearPlot(bool replot)
{
    _plot->detachItems();
    _plot->setTitle("");
    if (replot)
        _plot->replot();
}

void GraphContainer::blockPlot()
{
    // Add one progress layer
    ProgressLayer *layer = new ProgressLayer(this);
    layer->startAnimation();
    layer->show();
    layer->raise();
}

void GraphContainer::unblockPlot()
{
    // Find and remove topmost progress layer
    findChild<ProgressLayer *>()->deleteLater();
}

void GraphContainer::updatePlot(Queryable *entity, QList<NodeData *> dataSet)
{
    unblockPlot();
    clearPlot(false);

    if (!entity || dataSet.isEmpty())
        return;

    QDateTime minDateTime(dataSet[0]->dateTime().date());
    QHash< QString, QVector<QPointF> > curvePoints;

    foreach (NodeData *data, dataSet)
    {
        int x = minDateTime.secsTo(data->dateTime());
        foreach (QString key, data->keys())
        {
            if (!curvePoints.contains(key))
                curvePoints.insert(key, QVector<QPointF>());
            curvePoints[key].append(QPointF(x, data->value(key)));
        }
    }
    qDeleteAll(dataSet);

    _plot->setAxisScaleDraw(QwtPlot::xBottom, new DateTimeDraw(minDateTime));
    _plot->setAxisScaleDiv(QwtPlot::xBottom, _weekDiv);
    _plot->setAxisAutoScale(QwtPlot::yLeft);

    int i = 0;
    foreach (QString key, curvePoints.keys())
    {
        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setYAxis(QwtPlot::yLeft);
        QPen pen(QColor::fromHsvF(i++ / curvePoints.size(), 1.0, 1.0));
        pen.setWidth(2);
        curve->setPen(pen);
        curve->attach(_plot);
        curve->setSamples(curvePoints[key]);
        curve->setTitle(entity->nameForDataKey(key)
                        + TEXT_SEPARATOR
                        + entity->unitForDataKey(key));
    }

    _plot->replot();

    QString title;
    if (dynamic_cast<Node *>(entity))
    {
        Node *node = dynamic_cast<Node *>(entity);
        QString format(tr("%1, Sensor %2"));
        title = format.arg(node->line()->name(),
                           node->name().mid(1));
    }
    else if (dynamic_cast<Silo *>(entity))
    {
        Silo *silo = dynamic_cast<Silo *>(entity);
        QString format(tr("Averages for Silo %1"));
        title = format.arg(silo->name().mid(1));
    }
    else if (dynamic_cast<Location *>(entity))
    {
        Location *location = dynamic_cast<Location *>(entity);
        if (curvePoints.size())
        {
            QString format(tr("%1, %2"));
            QString name = location->sensorKeys()[curvePoints.keys()[0]];
            title = format.arg(location->name(), name);
        }
    }
    _plot->setTitle(title);

    CurveTrackingPicker *picker = dynamic_cast<CurveTrackingPicker *>(_picker);
    if (picker)
        picker->setReferenceDateTime(minDateTime);
}
