// SPDX-FileCopyrightText: 2017 Christian Sailer
// SPDX-FileCopyrightText: 2020 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "imainwindowmodule.h"
#include <memory>
#include <vector>

typedef std::vector<std::unique_ptr<IMainWindowModule>> MainWindowModuleVec;

class IMainWindowModuleFactory {
  public:
    virtual const MainWindowModuleVec &getModules() const = 0;
    virtual ~IMainWindowModuleFactory() {}
};
