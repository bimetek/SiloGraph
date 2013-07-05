/*****************************************************************************
 * SiloListView.h
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

#ifndef SILOLISTVIEW_H
#define SILOLISTVIEW_H

#include <QWidget>
class Location;
class Node;
class Silo;

class SiloListView : public QWidget
{
    Q_OBJECT
public:
    explicit SiloListView(QWidget *parent = 0);

public slots:
    void setLocation(Location *location);

signals:
    void targetSwitched(Node *node, Silo *silo);

private:
    Location *_currentLocation;
};

#endif // SILOLISTVIEW_H
