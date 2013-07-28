/*****************************************************************************
 * SharedSettings.cpp
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

#include "SharedSettings.h"
#include <QSettings>

static const char *LOGO_SIZE_RATIO_KEY = "logo_size_ratio";
static const char *SILO_MIN_HEIGHT_KEY = "silo_min_height";

SharedSettings::SharedSettings(QObject *parent) :
    QObject(parent)
{
    _settings = new QSettings("settings.ini", QSettings::IniFormat);
}

SharedSettings *SharedSettings::sharedSettings()
{
    static SharedSettings *sSharedObject = 0;
    if (!sSharedObject)
        sSharedObject = new SharedSettings();
    return sSharedObject;
}

qreal SharedSettings::logoSizeRatio() const
{
    static const qreal defaultValue = 0.5;

    bool ok = false;
    qreal v = _settings->value(LOGO_SIZE_RATIO_KEY, defaultValue).toReal(&ok);
    return ok ? v : defaultValue;
}

void SharedSettings::setLogoSizeRatio(const qreal &value)
{
    _settings->setValue(LOGO_SIZE_RATIO_KEY, value);
    _settings->sync();
    emit logoSizeRatioChanged(value);
}

int SharedSettings::siloMinimumHeight() const
{
    static const int defaultValue = 450;

    bool ok = false;
    int value = _settings->value(SILO_MIN_HEIGHT_KEY, defaultValue).toInt(&ok);
    return ok ? value : defaultValue;
}

void SharedSettings::setSiloMinimumHeight(const int &value)
{
    _settings->setValue(SILO_MIN_HEIGHT_KEY, value);
    _settings->sync();
    emit siloMinimumHeightChanged(value);
}

