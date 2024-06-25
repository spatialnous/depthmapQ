// SPDX-FileCopyrightText: 2017 Christian Sailer
// SPDX-FileCopyrightText: 2020 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindowmoduleregistry.hpp"

#include "modules/segmentshortestpaths/gui/segmentpathsmainwindow.h"
#include "modules/vgaparallel/gui/vgaparallelmainwindow.h"
#include "modules/vgapaths/gui/vgapathsmainwindow.h"

void MainWindowModuleRegistry::populateModules() {
    // Register any main window modules here
    REGISTER_MAIN_WINDOW_MODULE(SegmentPathsMainWindow);
    REGISTER_MAIN_WINDOW_MODULE(VGAParallelMainWindow);
    REGISTER_MAIN_WINDOW_MODULE(VGAPathsMainWindow);
    // *********
}
