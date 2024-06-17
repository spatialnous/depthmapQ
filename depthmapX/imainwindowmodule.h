// SPDX-FileCopyrightText: 2020 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QMenuBar>
#include <QObject>
#include <iostream>

class MainWindow;

class IMainWindowModule : public QObject {
    Q_OBJECT

  public:
    virtual bool createMenus(MainWindow *m_mainWindow) = 0;
    virtual ~IMainWindowModule() {}
};
