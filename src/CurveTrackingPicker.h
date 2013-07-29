/*****************************************************************************
 * CurveTrackingPicker.h
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

#ifndef CURVETRACKINGPICKER_H
#define CURVETRACKINGPICKER_H

#include <qwt_plot_picker.h>
#include <QDateTime>
#include <QPointF>
#include "Globals.h"
class QwtPlotItem;


class CurveTrackingPicker : public QwtPlotPicker
{
public:
    CurveTrackingPicker(
            int xAxis, int yAxis, RubberBand rubberBand, DisplayMode tracker,
            QWidget *canvas, const QDateTime &dateTime);
    void setReferenceDateTime(const QDateTime &dt);

protected:
    virtual QwtText trackerTextF(const QPointF &p) const;
    virtual void widgetMousePressEvent(QMouseEvent *e);
    virtual void widgetLeaveEvent(QEvent *e);
    virtual void widgetMouseMoveEvent(QMouseEvent *e);

private:
    QDateTime _datetime;
    QwtPlotItem *_grabbedItem;

    struct PositionContext
    {
        PositionContext() : point(R_NO_DATA, R_NO_DATA), item(0) {}
        QPointF point;
        QwtPlotItem *item;
    };

    PositionContext findNearest(const QPointF &p) const;
    QwtPlotItem *moveToNearest(const QPointF &p) const;

    QPointF mousePosition(QMouseEvent *e);
};

#endif // CURVETRACKINGPICKER_H
