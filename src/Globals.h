/*****************************************************************************
 * Globals.h
 *
 * Created: 04/7 2013 by uranusjr
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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <limits>
#include <QLayout>

#define W_SCALE 1       // Scale everything in UI

#define D_NO_DATA (-1 * std::numeric_limits<double>::max())

#include <QLocale>

void clearLayout(QLayout *layout);
QLocale::Language getCurrentLanguage();
QLocale::Country getCurrentCountry();
QLocale getCurrentLocale();
QFont defaultFontForSize(uint pointSize = 0);

#endif // GLOBALS_H
