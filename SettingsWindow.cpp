/*****************************************************************************
 * SettingsWindow.cpp
 *
 * Created: 22/7 2013 by uranusjr
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

#include "SettingsWindow.h"
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include "SharedSettings.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent)
{
    _settings = SharedSettings::sharedSettings();

    setAttribute(Qt::WA_DeleteOnClose);

    _logoRatio = new QSlider(Qt::Horizontal);
    _logoRatio->setMinimum(0);
    _logoRatio->setMaximum(100);
    _logoRatio->setMinimumWidth(300);
    _logoRatio->setTracking(true);
    _logoRatio->setValue(_settings->logoSizeRatio() * 100);
    connect(_logoRatio, SIGNAL(valueChanged(int)),
            this, SLOT(setLogoRatio(int)));

    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(new QLabel(tr("Logo size")), _logoRatio);

    _okButton = new QPushButton(tr("OK"));
    connect(_okButton, SIGNAL(clicked()), this, SLOT(accept()));

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(_okButton);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);
}

void SettingsWindow::setLogoRatio(int sliderValue)
{
    QSlider *slider = reinterpret_cast<QSlider *>(sender());
    qreal steps = slider->maximum() - slider->minimum();
    qreal ratio = sliderValue / steps;
    _settings->setLogoSizeRatio(ratio);
}
