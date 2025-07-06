// SPDX-FileCopyrightText: 2020-2024 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "qtgui/imainwindowmodule.hpp"

class VGAPathsMainWindow : public IMainWindowModule {

  private:
    enum PathType { VISUAL, ANGULAR, METRIC };

  private slots:
    void OnShortestPath(MainWindow *mainWindow, PathType pathType);
    void OnExtractLinkData(MainWindow *mainWindow);
    void OnMakeIsovistZones(MainWindow *mainWindow);
    void OnMetricShortestPathsToMany(MainWindow *mainWindow);

  public:
    VGAPathsMainWindow() : IMainWindowModule() {}
    bool createMenus(MainWindow *mainWindow);
};
