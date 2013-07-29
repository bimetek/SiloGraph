/*****************************************************************************
 * Globals.h
 *
 * Created: 04/7 2013 by uranusjr
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

#ifndef GLOBALS_H
#define GLOBALS_H

#include <limits>
#include <QtGlobal>
#include <QLocale>
#include <QLayout>

const double D_NO_DATA = std::numeric_limits<double>::max();
const qreal R_NO_DATA = std::numeric_limits<qreal>::max();
const QString DEGREE_SIGN = QString::fromLatin1("\xba", 1);
const QString TEXT_SEPARATOR = "___";

#ifdef Q_OS_MAC
    const QString SETTINGS_WINDOW_TITLE = "Preferences";
#else
    const QString SETTINGS_WINDOW_TITLE = "Settings";
#endif

double roundTo(double value);
void clearLayout(QLayout *layout);
QLocale::Language getCurrentLanguage();
QLocale::Country getCurrentCountry();
QLocale getCurrentLocale();
QFont defaultFontForSize(uint pointSize = 0);
QString textFromFile(QString filename);
QString capitalized(const QString &input, bool toLower = true);

#endif // GLOBALS_H
