/*****************************************************************************
 * SiloListView.cpp
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

#include "SiloListView.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QTimerEvent>
#include <QSignalMapper>
#include "Globals.h"
#include "Location.h"
#include "Silo.h"
#include "NodeLine.h"
#include "Node.h"
#include "SiloView.h"
#include "LogoHolder.h"
#include <QDebug>

SiloListView::SiloListView(QWidget *parent) :
    QWidget(parent),
    _currentLocation(0),
    _pollingTimerId(0)
{
    _logo = new LogoHolder(":/img/logo_transparent.png", this);
    _sensorsMapper = new QSignalMapper(this);
    connect(_sensorsMapper, SIGNAL(mapped(QString)),
            this, SLOT(locationSensorClicked(QString)));

    _siloListLayout = new QHBoxLayout();
    _titleLayout = new QHBoxLayout();
    _titleLayout->setContentsMargins(0, 0, 0, 0);
    _titleLayout->setAlignment(Qt::AlignBottom);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(_titleLayout, 0);
    mainLayout->addLayout(_siloListLayout, 1);
    setLayout(mainLayout);
}

void SiloListView::setLocation(Location *location)
{
    if (_currentLocation == location)
        return;

    _currentLocation = location;

    QLabel *title = new QLabel(location->name());
    title->setStyleSheet("font-size: 20px; font-weight: bold; height: 16px;");

    clearLayout(_titleLayout);
    _titleLayout->addStretch(1);
    _titleLayout->addWidget(title);

    QString buttonStyle = textFromFile(":/assets/silo_button_styles.css");
    foreach (QString key, location->sensorKeys().keys())
    {
        _titleLayout->addSpacing(20);
        QPushButton *button = new QPushButton(location->sensorKeys()[key]);
        button->setObjectName(key);
        button->setStyleSheet(buttonStyle);
        _titleLayout->addWidget(button);
        connect(button, SIGNAL(clicked()), _sensorsMapper, SLOT(map()));
        _sensorsMapper->setMapping(button, key);
    }
    _titleLayout->addStretch(1);

    clearLayout(_siloListLayout);
    foreach (Silo *silo, location->silos())
    {
        SiloView *sv = new SiloView(silo);
        _siloListLayout->addWidget(sv);
        connect(sv, SIGNAL(targetSwitched(Queryable *, QString)),
                this, SIGNAL(targetSwitched(Queryable *, QString)));
        connect(this, SIGNAL(shouldPollForLocation(Location *)),
                sv, SLOT(invalidateLastUpdate()));
    }

    if (_pollingTimerId)
        killTimer(_pollingTimerId);
    _pollingTimerId = startTimer(5 * 60 * 1000);    // ms

    if (_currentLocation)
        emit shouldPollForLocation(_currentLocation);   // Poll now
}

void SiloListView::timerEvent(QTimerEvent *e)
{
    if (e->timerId() == _pollingTimerId)
    {
        if (_currentLocation)
            emit shouldPollForLocation(_currentLocation);
    }
}

void SiloListView::resizeEvent(QResizeEvent *)
{
    resizeLogo(logoSizeRatio());
}

void SiloListView::updateLatestData(NodeLine *line, QList<double> data,
                                    QDateTime dateTime)
{
    // If the fetched data is out of date and is from some previous location,
    // ignore it.
    if (_currentLocation != line->silo()->location())
        return;

    Silo *silo = line->silo();
    SiloView *sv = findChild<SiloView *>(silo->name());
    sv->updateLatestData(line, data, dateTime);
}

void SiloListView::updateLatestData(Location *location,
                                    QHash<QString, double> data, QDateTime)
{
    if (_currentLocation != location)
        return;

    QString format("%1%2");
    QString suffix;
    foreach (QString key, data.keys())
    {
        QString sensorName = location->sensorKeys()[key];
        QPushButton *button = findChild<QPushButton *>(key);
        if (button)
        {
            if (sensorName.compare("temperature", Qt::CaseInsensitive) == 0)
                suffix = DEGREE_SIGN;
            else if (sensorName.compare("humidity", Qt::CaseInsensitive) == 0)
                suffix = "%";
            else
                suffix = "";
            button->setText(format.arg(QString::number(data[key]), suffix));
        }
    }
}

void SiloListView::setMinimumHeight(int minh)
{
    QWidget::setMinimumHeight(minh);
}

void SiloListView::resizeLogo(qreal ratio)
{
    qreal width = size().width() * ratio;
    qreal height = size().height() * ratio;
    if (width / height > _logo->aspect())   // too wide
        width = height * _logo->aspect();
    else                                    // too tall
        height = width / _logo->aspect();
    _logo->setGeometry((size().width() - width) / 2,
                       (size().height() - height) / 2,
                       width, height);
}

void SiloListView::locationSensorClicked(QString name)
{
    emit targetSwitched(_currentLocation, name);
}
