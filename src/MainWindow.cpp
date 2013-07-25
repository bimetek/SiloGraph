/*****************************************************************************
 * MainWindow.cpp
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

#include "MainWindow.h"
#include <QMenu>
#include <QMenuBar>
#include "Globals.h"
#include "MainWidget.h"
#include "SettingsWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), _settings(0)
{
    MainWidget *mainWidget = new MainWidget(this);
    setCentralWidget(mainWidget);
    setWindowIcon(QIcon(":/img/logo.png"));
    setWindowTitle("Silo Graph");

    QMenu *tools = new QMenu("Tools");
    tools->addAction("Settings", this, SLOT(showSettings()));

    menuBar()->addMenu(tools);

    showMaximized();
}

MainWindow::~MainWindow()
{

}

void MainWindow::showSettings()
{
    if (!_settings)
    {
        _settings = new SettingsWindow();
        connect(_settings, SIGNAL(destroyed()), this, SLOT(cleanSettings()));
        _settings->show();
    }
    _settings->activateWindow();
}

void MainWindow::cleanSettings()
{
    _settings = 0;
}
