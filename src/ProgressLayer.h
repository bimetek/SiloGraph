/*****************************************************************************
 * ProgressLayer.h
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

#ifndef PROGRESSLAYER_H
#define PROGRESSLAYER_H

#include <QWidget>
class QProgressIndicator;

class ProgressLayer : public QWidget
{
    Q_OBJECT

public:
    explicit ProgressLayer(QWidget *parent = 0);
    void startAnimation();
    void stopAnimation();
    inline void setAnimated(bool value)
    {
        if (value)
            startAnimation();
        else
            stopAnimation();
    }
    bool isAnimated() const;

private:
    QProgressIndicator *_indicator;
};

#endif // PROGRESSLAYER_H
