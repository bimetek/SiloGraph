/*****************************************************************************
 * SiloListView.h
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

#ifndef SILOLISTVIEW_H
#define SILOLISTVIEW_H

#include <QWidget>
#include <QDateTime>
#include <QList>
#include <QPushButton>
class QHBoxLayout;
class QSignalMapper;
class QVBoxLayout;
class Queryable;
class Location;
class NodeLine;
class LogoHolder;

class SiloListView : public QWidget
{
    Q_OBJECT

public:
    explicit SiloListView(QWidget *parent = 0);

    qreal logoSizeRatio() const { return _logoSizeRatio; }
    void setLogoSizeRatio(const qreal &value)
    {
        _logoSizeRatio = value;
        resizeLogo(value);
    }

public slots:
    void setLocation(Location *location);
    void updateLatestData(NodeLine *line, QList<double> data,
                          QDateTime dateTime);
    void updateLatestData(Location *location,
                          QHash<QString, double> data, QDateTime);
    void setMinimumHeight(int minh);
    void resizeLogo(qreal ratio);

protected:
    virtual void timerEvent(QTimerEvent *e);
    virtual void resizeEvent(QResizeEvent *);

signals:
    void targetSwitched(Queryable *entity, QString key);
    void shouldPollForLocation(Location *location);

private slots:
    void locationSensorClicked(QString name);

private:
    Location *_currentLocation;
    int _pollingTimerId;
    LogoHolder *_logo;
    qreal _logoSizeRatio;
    QSignalMapper *_sensorsMapper;
    QHBoxLayout *_siloListLayout;
    QHBoxLayout *_titleLayout;
};

#endif // SILOLISTVIEW_H
