/*****************************************************************************
 * LogoHolder.h
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

#ifndef LOGOHOLDER_H
#define LOGOHOLDER_H

#include <QLabel>

class LogoHolder : public QLabel
{
    Q_OBJECT

public:
    explicit LogoHolder(QString filePath, QWidget *parent = 0);
    inline qreal aspect() const { return _aspect; }

protected:
    virtual bool hasHeightForWidth() const;
    virtual int heightForWidth(int w) const;

private:
    qreal _aspect;      // width / height
};

#endif // LOGOHOLDER_H
