// SPDX-FileCopyrightText: 2020 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "qtgui/imainwindowmodule.hpp"

class SegmentPathsMainWindow : public IMainWindowModule {

  private:
    enum PathType { ANGULAR, METRIC, TOPOLOGICAL };

  private slots:
    void OnShortestPath(MainWindow *mainWindow, PathType pathType);

  public:
    SegmentPathsMainWindow() : IMainWindowModule() {}
    bool createMenus(MainWindow *mainWindow);
};
