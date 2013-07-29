/*****************************************************************************
 * Globals.cpp
 *
 * Created: 05/7 2013 by uranusjr
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

#include "Globals.h"
#include <cmath>
#include <QApplication>
#include <QFile>
#include <QWidget>

double roundTo(double value)
{
    return value < 0.0 ? ceil(value - 0.5) : floor(value + 0.5);
}

void clearLayout(QLayout *layout)
{
    QLayoutItem *item = layout->takeAt(0);
    while (item)
    {
        if (item->layout())
        {
            clearLayout(item->layout());
            item->layout()->deleteLater();
        }
        item->widget()->deleteLater();
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

QString textFromFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QString text(file.readAll());
    file.close();
    return text;
}

QString capitalized(const QString &input, bool toLower)
{
    if (input.isEmpty())
        return input;
    QString output = input;
    if (toLower)
        output = output.toLower();
    output[0] = output[0].toUpper();
    return output;
}
