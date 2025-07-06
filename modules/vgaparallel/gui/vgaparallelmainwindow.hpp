// SPDX-FileCopyrightText: 2020-2024 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "qtgui/imainwindowmodule.hpp"

class VGAParallelMainWindow : public IMainWindowModule {
  private:
    enum class AnalysisType {
        NONE,
        VISUAL_LOCAL_OPENMP,
        VISUAL_LOCAL_ADJMATRIX,
        VISUAL_GLOBAL_OPENMP,
        METRIC_OPENMP,
        ANGULAR_OPENMP
    };
    double ConvertForVisibility(const std::string &radius) const;
    double ConvertForMetric(const std::string &radius) const;

  private slots:
    void OnVGAParallel(MainWindow *mainWindow, VGAParallelMainWindow::AnalysisType analysisType);

  public:
    VGAParallelMainWindow() : IMainWindowModule() {}
    bool createMenus(MainWindow *mainWindow);
};
