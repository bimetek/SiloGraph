/*****************************************************************************
 * Globals.cpp
 *
 * Created: 05/7 2013 by uranusjr
 *
 * Copyright 2013 uranusjr. All rights reserved.
 *
 * This file is published under the Creative Commons 3.0.
 * http://creativecommons.org/licenses/by/3.0/
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/

#include "Globals.h"
#include <QApplication>
#include <QWidget>

void clearLayout(QLayout *layout)
{
    QLayoutItem *item = layout->takeAt(0);
    while (item)
    {
        if (item->layout())
        {
            clearLayout(item->layout());
            delete item->layout();
        }
        delete item->widget();
        item = layout->takeAt(0);
    }
}

QLocale::Language getCurrentLanguage()
{
    return QLocale::English;
}

QLocale::Country getCurrentCountry()
{
    return QLocale::UnitedStates;
}

QLocale getCurrentLocale()
{
    return QLocale(getCurrentLanguage(), getCurrentCountry());
}

QFont defaultFontForSize(uint pointSize)
{
    QFont font = QApplication::font();
    if (pointSize > 0)
        font.setPointSize(pointSize);
    return font;
}
