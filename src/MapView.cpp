/*****************************************************************************
 * MapView.cpp
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
