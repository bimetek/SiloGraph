/*****************************************************************************
 * LogoHolder.cpp
 *
 * Created: 16/7 2013 by uranusjr
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

#include "LogoHolder.h"
#include <QStyle>
#include <QDebug>

LogoHolder::LogoHolder(QString filePath, QWidget *parent) :
    QLabel(parent)
{
    QString ss = "border-image: url(%1) 0 0 0 0 stretch stretch;";
    setStyleSheet(ss.arg(filePath));

    QPixmap img(filePath);
    qreal width = img.size().width();
    qreal height = img.size().height();
    _aspect = width / height;

    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    policy.setHeightForWidth(true);
    setSizePolicy(policy);
}

bool LogoHolder::hasHeightForWidth() const
{
    return true;
}

int LogoHolder::heightForWidth(int w) const
{
    return (w / _aspect);
}
