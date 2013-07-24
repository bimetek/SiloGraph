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
#include <limits>
#include <QDateTime>
#include <QGridLayout>
#include <QMouseEvent>
#include <QPointF>
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
#include "Silo.h"
#include "NodeData.h"
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
        return QwtText(QString("%1\n%3%2")
                       .arg(dt.toString("yyyy-MM-dd HH:mm:ss"),
                            QString::fromLatin1("\xba", 1),     // degree sign
                            QString::number(p.y(), 'f', 2)));
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
        QPointF nearest(std::numeric_limits<qreal>::max(),
                        std::numeric_limits<qreal>::max());
        foreach (QwtPlotItem *it, plot()->itemList(QwtPlotItem::Rtti_PlotCurve))
        {
            QwtPlotCurve *curve = dynamic_cast<QwtPlotCurve *>(it);
            QPointF current(std::numeric_limits<qreal>::max(),
                            std::numeric_limits<qreal>::max());

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
        int x = roundTo(plot()->transform(QwtPlot::xBottom, nearest.x()));
        int y = roundTo(plot()->transform(QwtPlot::yLeft, nearest.y()));

        QCursor::setPos(canvas()->mapToGlobal(QPoint(x, y)));
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

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(_plot, 0, 0);
    setLayout(mainLayout);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    setMinimumSize(200, 150);
    _plot->setMinimumSize(200, 150);
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
    if (replot)
        _plot->replot();
}

void GraphContainer::updatePlot(Node *node, Silo *silo,
                                QList <QList<NodeData *> > dataSets)
{
    Q_UNUSED(node);
    Q_UNUSED(silo);

    clearPlot(false);

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
    QDateTime cutoff(firstDateTime.date());

    _plot->setAxisScaleDraw(QwtPlot::xBottom, new DateTimeDraw(cutoff));
    _plot->setAxisScaleDiv(QwtPlot::xBottom, _weekDiv);
    _plot->setAxisAutoScale(QwtPlot::yLeft);

    int i = 0;
    double dataCount = dataSets.size();
    foreach (QList<NodeData *> dataSet, dataSets)
    {
        if (dataSet.isEmpty())
            continue;

        QVector<QPointF> points;
        foreach (NodeData *data, dataSet)
        {
            QPointF point(cutoff.secsTo(data->dateTime()),
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

    _plot->replot();

//    if (_panner)
//        _panner->deleteLater();
//    _panner = new LimitedPanner(_plot->canvas());
//    if (_magnifier)
//        _magnifier->deleteLater();
//    _magnifier = new LimitedMagnifier(_plot->canvas());
    if (_picker)
        _picker->deleteLater();
    _picker = new CurveTrackingPicker(
                QwtPlot::xBottom, QwtPlot::yLeft,
                QwtPlotPicker::NoRubberBand, QwtPicker::AlwaysOn,
                _plot->canvas(), cutoff);
}
