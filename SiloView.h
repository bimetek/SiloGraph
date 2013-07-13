/*****************************************************************************
 * SiloView.h
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

#ifndef SILOVIEW_H
#define SILOVIEW_H

#include <QWidget>
class Silo;
class Node;
class NodeLine;

class SiloView : public QWidget
{
    Q_OBJECT

public:
    explicit SiloView(Silo *silo, QWidget *parent = 0);

public slots:
    void updateLatestData(NodeLine *line, QList<double> &data);

signals:
    void targetSwitched(Node *node, Silo *silo);

private slots:
    void switchToNode(QObject *obj);

private:
    Silo *_silo;
};

#endif // SILOVIEW_H
