/*****************************************************************************
 * CurveTrackingPicker.cpp
 *
 * Created: 29/7 2013 by uranusjr
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

#include "CurveTrackingPicker.h"
#include <QMouseEvent>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>


CurveTrackingPicker::CurveTrackingPicker(
        int xAxis, int yAxis, QwtPicker::RubberBand rubberBand,
        QwtPicker::DisplayMode tracker, QWidget *canvas,
        const QDateTime &dateTime)
    : QwtPlotPicker(xAxis, yAxis, rubberBand, tracker, canvas), _grabbedItem(0)
{
    setReferenceDateTime(dateTime);
}

void CurveTrackingPicker::setReferenceDateTime(const QDateTime &dateTime)
{
    _datetime = dateTime;
}

QwtText CurveTrackingPicker::trackerTextF(const QPointF &p) const
{
    if (!_grabbedItem)
        return QwtText();

    QDateTime dt = _datetime.addSecs(p.x());
    QString format = QString(" %1 \n   %2: %3%4   ");
    QString itemName;
    QString suffix;
    if (_grabbedItem)
    {
        QStringList comp = _grabbedItem->title().text().split(TEXT_SEPARATOR);
        itemName = comp.first();
        if (comp.size() > 1)
            suffix = comp.last();
    }
    QwtText result(format.arg(dt.toString("yyyy-MM-dd HH:mm:ss"),
                              capitalized(itemName),
                              QString::number(p.y(), 'f', 2),
                              suffix),
                   QwtText::PlainText);
    QBrush whiteBrush(Qt::white);
    result.setBackgroundBrush(whiteBrush);
    result.setFont(QFont("sans-sarif"));
    return result;
}

void CurveTrackingPicker::widgetMousePressEvent(QMouseEvent *e)
{
    QwtPlotPicker::widgetMousePressEvent(e);
    if (!_grabbedItem)
        _grabbedItem = moveToNearest(mousePosition(e));
    else
        _grabbedItem = 0;
}

void CurveTrackingPicker::widgetLeaveEvent(QEvent *e)
{
    QwtPlotPicker::widgetLeaveEvent(e);
    _grabbedItem = 0;
}

void CurveTrackingPicker::widgetMouseMoveEvent(QMouseEvent *e)
{
    QwtPlotPicker::widgetMouseMoveEvent(e);
    if (!_grabbedItem)
        return;
    _grabbedItem = moveToNearest(mousePosition(e));
}

CurveTrackingPicker::PositionContext CurveTrackingPicker::findNearest(
        const QPointF &p) const
{
    CurveTrackingPicker::PositionContext context;
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
        if (fabs(current.y() - p.y()) < fabs(context.point.y() - p.y()))
        {
            context.point = current;
            context.item = it;
        }
    }
    return context;
}

QwtPlotItem *CurveTrackingPicker::moveToNearest(const QPointF &p) const
{
    double vx = plot()->invTransform(QwtPlot::xBottom, p.x());
    double vy = plot()->invTransform(QwtPlot::yLeft, p.y());
    CurveTrackingPicker::PositionContext context = findNearest(QPointF(vx, vy));
    QPointF &nearest = context.point;
    if (nearest.x() != R_NO_DATA && nearest.y() != R_NO_DATA)
    {
        int x = roundTo(plot()->transform(QwtPlot::xBottom, nearest.x()));
        int y = roundTo(plot()->transform(QwtPlot::yLeft, nearest.y()));
        QCursor::setPos(canvas()->mapToGlobal(QPoint(x, y)));
    }

    return context.item;
}

QPointF CurveTrackingPicker::mousePosition(QMouseEvent *e)
{
#if QT_VERSION >= 0x050000
    return e->localPos();
#else
    return e->posF();
#endif
}
