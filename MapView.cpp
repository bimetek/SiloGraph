/*****************************************************************************
 * MapView.cpp
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

#include "MapView.h"
#include <QHBoxLayout>
#include <QWebFrame>
#include <QWebView>
#include "MapMarker.h"
#include <QDebug>

MapView::MapView(QWidget *parent)
    : QWidget(parent)
{
    _webView = new QWebView();
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setMargin(0);
    layout->addWidget(_webView);
    setLayout(layout);

    _webView->load(QUrl("qrc:///web/map.html"));
    connect(_webView, SIGNAL(loadFinished(bool)), this, SIGNAL(mapLoaded()));
    connect(_webView->page()->mainFrame(),
            SIGNAL(javaScriptWindowObjectCleared()),
            this, SLOT(addObjectReference()));
}

void MapView::addObjectReference()
{
    _webView->page()->mainFrame()->addToJavaScriptWindowObject("qMapView",
                                                               this);
}

void MapView::resizeEvent(QResizeEvent *)
{
    frame()->evaluateJavaScript("refit();");
}

QWebFrame *MapView::frame()
{
    return _webView->page()->mainFrame();
}

void MapView::appendMarker(MapMarker *marker)
{
    marker->setParent(this);

    QString script = QString("appendMarker(\"%1\", %2, %3);")
            .arg(marker->name(),
                 QString::number(marker->latitude()),
                 QString::number(marker->longitude()));
    int index = frame()->evaluateJavaScript(script).toInt();

    _markers[index] = marker;
}

void MapView::onMarkerClicked(int index)
{
    if (_markers.contains(index))
        emit markerClicked(_markers[index]);
}
