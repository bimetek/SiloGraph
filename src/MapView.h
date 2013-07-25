/*****************************************************************************
 * MapView.h
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

#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QWidget>
#include <QHash>
class QWebFrame;
class QWebView;
class MapMarker;

class MapView : public QWidget
{
    Q_OBJECT

public:
    explicit MapView(QWidget *parent = 0);

public slots:
    void appendMarker(MapMarker *marker);
    void onMarkerClicked(int index);

signals:
    void mapLoaded();
    void markerClicked(MapMarker *marker);

protected:
    virtual void resizeEvent(QResizeEvent *);

private slots:
    void addObjectReference();

private:
    QHash<int, MapMarker *> _markers;
    QWebView *_webView;
    QWebFrame *frame();
};

#endif // MAPVIEW_H
