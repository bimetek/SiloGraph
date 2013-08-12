/*****************************************************************************
 * SharedSettings.h
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

#ifndef SHAREDSETTINGS_H
#define SHAREDSETTINGS_H

#include <QObject>
class QSettings;

class SharedSettings : public QObject
{
    Q_OBJECT

public:
    explicit SharedSettings(QObject *parent = 0);
    static SharedSettings *sharedSettings();

    qreal logoSizeRatio() const;
    int siloMinimumHeight() const;
    int mapMinimumWidth() const;
    int lastUpdateLabelHeight() const;

public slots:
    void setLogoSizeRatio(const qreal &value);
    void setSiloMinimumHeight(const int value);
    void setMapMinimumWidth(const int value);
    void setLastUpdateLabelHeight(const int value);

signals:
    void logoSizeRatioChanged(qreal value);
    void siloMinimumHeightChanged(int value);
    void mapMinimumWidthChanged(int value);
    void lastUpdateLabelHeightChanged(int value);

private:
    QSettings *_settings;
};

#endif // SHAREDSETTINGS_H
