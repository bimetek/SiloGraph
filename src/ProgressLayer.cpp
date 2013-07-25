/*****************************************************************************
 * ProgressLayer.cpp
 *
 * Created: 25/7 2013 by uranusjr
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

#include "ProgressLayer.h"
#include <QHBoxLayout>
#include <QPalette>
#include <QVBoxLayout>
#include <QProgressIndicator/QProgressIndicator.h>

ProgressLayer::ProgressLayer(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette newPalette(palette());
    newPalette.setColor(QPalette::Background, QColor(0, 0, 0, 50));
    setPalette(newPalette);

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    _indicator = new QProgressIndicator();
    _indicator->setFixedSize(70, 70);

    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addStretch(1);
    hbox->addWidget(_indicator);
    hbox->addStretch(1);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addStretch(1);
    vbox->addLayout(hbox);
    vbox->addStretch(1);

    setLayout(vbox);

    if (parent)
        resize(parent->size());
}

void ProgressLayer::startAnimation()
{
    _indicator->startAnimation();
}

void ProgressLayer::stopAnimation()
{
    _indicator->stopAnimation();
}

bool ProgressLayer::isAnimated() const
{
    return _indicator->isAnimated();
}
