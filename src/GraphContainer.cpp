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
#include <limits>
#include <QDateTime>
#include <QGridLayout>
#include <QMouseEvent>
#include <QPointF>
#include <QResizeEvent>
#include <QVector>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_scale_div.h>
#include <qwt_scale_draw.h>
#include <qwt_scale_engine.h>
#include <qwt_series_data.h>
#include "Globals.h"
#include "Node.h"
#include "NodeLine.h"
#include "Silo.h"
#include "Location.h"
#include "NodeData.h"
#include "ProgressLayer.h"
#include <QDebug>


class CurveTrackingPicker : public QwtPlotPicker
{
public:
    CurveTrackingPicker(int xAxis, int yAxis, RubberBand rubberBand,
                        DisplayMode trackerMode, QWidget *canvas, QDateTime dt)
        : QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, canvas),
          _grapped(false)
    {
        setReferenceDateTime(dt);
    }
    void setReferenceDateTime(QDateTime dt) { _datetime = dt; }

protected:
    virtual QwtText trackerTextF(const QPointF &p) const
    {
        if (!_grapped)
            return QwtText();

        QDateTime dt = _datetime.addSecs(p.x());
        QString format = QString(" %1 \n%2%3");

        QwtText result(format.arg(dt.toString("yyyy-MM-dd HH:mm:ss"),
                                  QString::number(p.y(), 'f', 2),
                                  DEGREE_SIGN));
        QBrush whiteBrush(Qt::white);
        result.setBackgroundBrush(whiteBrush);
        return result;
    }
    virtual void widgetMousePressEvent(QMouseEvent *e)
    {
        QwtPlotPicker::widgetMousePressEvent(e);
        _grapped = !_grapped;
        moveToNearest(mousePosition(e));
    }
    virtual void widgetLeaveEvent(QEvent *e)
    {
        QwtPlotPicker::widgetLeaveEvent(e);
        _grapped = false;
    }
    virtual void widgetMouseMoveEvent(QMouseEvent *e)
    {
        QwtPlotPicker::widgetMouseMoveEvent(e);
        if (!_grapped)
            return;
        moveToNearest(mousePosition(e));
    }

private:
    bool _grapped;
    QDateTime _datetime;

#if QT_VERSION >= 0x050000
    inline QPointF mousePosition(QMouseEvent *e) { return e->localPos(); }
#else
    inline QPointF mousePosition(QMouseEvent *e) { return e->posF(); }
#endif

    inline QPointF findNearest(const QPointF &p) const
    {
        QPointF nearest(R_NO_DATA, R_NO_DATA);
        foreach (QwtPlotItem *it, plot()->itemList(QwtPlotItem::Rtti_PlotCurve))
        {
            QwtPlotCurve *curve = dynamic_cast<QwtPlotCurve *>(it);
            QPointF current(R_NO_DATA, R_NO_DATA);

            // Try to find the best point on this curve, fill it into "current"
            if (curve->sample(0).x() >= p.x())
            {
                current = curve->sample(0);
            }
            else if (curve->sample((int)curve->dataSize() - 1).x() < p.x())
            {
                current = curve->sample((int)curve->dataSize() - 1);
            }
            else
            {
                for (size_t i = 1; i < curve->dataSize(); i++)
                {
                    QPointF s1 = curve->sample(static_cast<int>(i - 1));
                    QPointF s2 = curve->sample(static_cast<int>(i));
                    if (s1.x() < p.x() && s2.x() >= p.x())
                    {
                        if (fabs(s1.x() - p.x()) <= fabs(s2.x() - p.x()))
                            current = s1;
                        else
                            current = s2;
                        break;
                    }
                }
            }

            // Update global nearest if "current" is better
            if (fabs(current.y() - p.y()) < fabs(nearest.y() - p.y()))
                nearest = current;
        }
        return nearest;
    }

    inline void moveToNearest(QPointF p)
    {
        double vx = plot()->invTransform(QwtPlot::xBottom, p.x());
        double vy = plot()->invTransform(QwtPlot::yLeft, p.y());
        QPointF nearest = findNearest(QPointF(vx, vy));
        if (nearest.x() != R_NO_DATA && nearest.y() != R_NO_DATA)
        {
            int x = roundTo(plot()->transform(QwtPlot::xBottom, nearest.x()));
            int y = roundTo(plot()->transform(QwtPlot::yLeft, nearest.y()));
            QCursor::setPos(canvas()->mapToGlobal(QPoint(x, y)));
        }
    }
};


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

    if (dataSet.isEmpty())
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


//    if (_panner)
//        _panner->deleteLater();
//    _panner = new LimitedPanner(_plot->canvas());
//    if (_magnifier)
//        _magnifier->deleteLater();
//    _magnifier = new LimitedMagnifier(_plot->canvas());
    CurveTrackingPicker *picker = dynamic_cast<CurveTrackingPicker *>(_picker);
    if (picker)
        picker->setReferenceDateTime(minDateTime);
}
