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
#include <QSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include "SharedSettings.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent)
{
    _settings = SharedSettings::sharedSettings();

    setAttribute(Qt::WA_DeleteOnClose);

    // The form
    QSlider *logoRatio = new QSlider(Qt::Horizontal);
    logoRatio->setMinimum(0);
    logoRatio->setMaximum(100);
    logoRatio->setMinimumWidth(300);
    logoRatio->setTracking(true);
    logoRatio->setValue(_settings->logoSizeRatio() * 100);
    connect(logoRatio, SIGNAL(valueChanged(int)),
            this, SLOT(setLogoRatio(int)));

    QSpinBox *siloMinHeight = new QSpinBox();
    siloMinHeight->setMinimum(1);
    siloMinHeight->setMaximum(1000);
    siloMinHeight->setValue(_settings->siloMinimumHeight());
    connect(siloMinHeight, SIGNAL(valueChanged(int)),
            _settings, SLOT(setSiloMinimumHeight(int)));

    QSpinBox *mapMinWidth = new QSpinBox();
    mapMinWidth->setMinimum(1);
    mapMinWidth->setMaximum(1000);
    mapMinWidth->setValue(_settings->mapMinimumWidth());
    connect(mapMinWidth, SIGNAL(valueChanged(int)),
            _settings, SLOT(setMapMinimumWidth(int)));

    // Button under form
    QPushButton *okButton = new QPushButton(tr("Close"));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    // Layout everything
    QFormLayout *formLayout = new QFormLayout();
    formLayout->addRow(new QLabel(tr("Logo size")), logoRatio);
    formLayout->addRow(new QLabel(tr("Minimum silo height")), siloMinHeight);
    formLayout->addRow(new QLabel(tr("Minimum map width")), mapMinWidth);

    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(okButton);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);
}

void SettingsWindow::setLogoRatio(int sliderValue)
{
    QSlider *slider = dynamic_cast<QSlider *>(sender());
    if (!slider)
        return;
    qreal steps = slider->maximum() - slider->minimum();
    qreal ratio = sliderValue / steps;
    _settings->setLogoSizeRatio(ratio);
}
