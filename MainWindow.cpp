/*****************************************************************************
 * MainWindow.cpp
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

#include "MainWindow.h"
#include "Globals.h"
#include "MainWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(new MainWidget(this));
    setFixedSize(1020 * W_SCALE, 480 * W_SCALE);
}

MainWindow::~MainWindow()
{

}
