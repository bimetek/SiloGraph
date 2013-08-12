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
static const char *MAP_MIN_WIDTH_KEY = "map_min_width";
static const char *LAST_UPDATE_HEIGHT_KEY = "last_update_height";

static const qreal LOGO_SIZE_RATIO_DEFAULT = 0.5;
static const int SILO_MIN_HEIGHT_DEFAULT = 450;
static const int MAP_MIN_WIDTH_DEFAULT = 200;
static const int LAST_UPDATE_HEIGHT_DEFAULT = 40;

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
    bool ok = false;
    qreal v = _settings->value(LOGO_SIZE_RATIO_KEY,
                               LOGO_SIZE_RATIO_DEFAULT).toReal(&ok);
    return ok ? v : LOGO_SIZE_RATIO_DEFAULT;
}

void SharedSettings::setLogoSizeRatio(const qreal &value)
{
    _settings->setValue(LOGO_SIZE_RATIO_KEY, value);
    _settings->sync();
    emit logoSizeRatioChanged(value);
}

int SharedSettings::siloMinimumHeight() const
{
    bool ok = false;
    int value = _settings->value(SILO_MIN_HEIGHT_KEY,
                                 SILO_MIN_HEIGHT_DEFAULT).toInt(&ok);
    return ok ? value : SILO_MIN_HEIGHT_DEFAULT;
}

void SharedSettings::setSiloMinimumHeight(const int value)
{
    _settings->setValue(SILO_MIN_HEIGHT_KEY, value);
    _settings->sync();
    emit siloMinimumHeightChanged(value);
}

int SharedSettings::mapMinimumWidth() const
{
    bool ok = false;
    int value = _settings->value(MAP_MIN_WIDTH_KEY,
                                 MAP_MIN_WIDTH_DEFAULT).toInt(&ok);
    return ok ? value : MAP_MIN_WIDTH_DEFAULT;
}

void SharedSettings::setMapMinimumWidth(const int value)
{
    _settings->setValue(MAP_MIN_WIDTH_KEY, value);
    _settings->sync();
    emit mapMinimumWidthChanged(value);
}

int SharedSettings::lastUpdateLabelHeight() const
{
    bool ok = false;
    int value = _settings->value(LAST_UPDATE_HEIGHT_KEY,
                                 LAST_UPDATE_HEIGHT_DEFAULT).toInt(&ok);
    return ok ? value : LAST_UPDATE_HEIGHT_DEFAULT;
}

void SharedSettings::setLastUpdateLabelHeight(const int value)
{
    _settings->setValue(LAST_UPDATE_HEIGHT_KEY, value);
    _settings->sync();
    emit lastUpdateLabelHeightChanged(value);
}


