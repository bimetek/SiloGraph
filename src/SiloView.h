/*****************************************************************************
 * SiloView.h
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

#ifndef SILOVIEW_H
#define SILOVIEW_H

#include <QWidget>
#include <QDateTime>
class QLabel;
class Queryable;
class Silo;
class NodeLine;

class SiloView : public QWidget
{
    Q_OBJECT

public:
    explicit SiloView(Silo *silo, QWidget *parent = 0);

public slots:
    void updateLatestData(NodeLine *line, QList<double> &data,
                          QDateTime dateTime);
    void refreshLastUpdate(QDateTime &dateTime);
    void refreshLastUpdate(QString text, bool clearAll = false);
    void invalidateLastUpdate();

signals:
    void targetSwitched(Queryable *entity, QString key = QString());

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void switchToObject(QObject *obj);

private:
    Silo *_silo;
    QLabel *_lastUpdate;
    QLabel *_backgroundHolder;
};

#endif // SILOVIEW_H
