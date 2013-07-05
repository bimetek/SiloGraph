/*****************************************************************************
 * Silo.cpp
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

#include "Silo.h"
#include "NodeLine.h"

Silo::Silo(QObject *parent) :
    QObject(parent)
{
}

 void Silo::addLine(NodeLine *line)
 {
     lines().append(line);
     line->setSilo(this);
 }
