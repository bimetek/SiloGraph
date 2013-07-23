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
    static qreal defaultValue = 0.5;

    bool ok = false;
    qreal value = _settings->value("logo_size_ratio", defaultValue).toReal(&ok);
    return ok ? value : defaultValue;
}

void SharedSettings::setLogoSizeRatio(const qreal &value)
{
    _settings->setValue("logo_size_ratio", value);
    _settings->sync();
    emit logoSizeRatioChanged(value);
}
