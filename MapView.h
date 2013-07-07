/*****************************************************************************
 * MapView.h
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

private slots:
    void addObjectReference();

private:
    QHash<int, MapMarker *> _markers;
    QWebView *_webView;
    QWebFrame *frame();
};

#endif // MAPVIEW_H
