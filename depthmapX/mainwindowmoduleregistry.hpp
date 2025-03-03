// SPDX-FileCopyrightText: 2017 Christian Sailer
// SPDX-FileCopyrightText: 2020 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "imainwindowmodulefactory.hpp"

#include <memory>
#include <vector>

class MainWindowModuleRegistry : public IMainWindowModuleFactory {
  public:
    MainWindowModuleRegistry() { populateModules(); }

    const MainWindowModuleVec &getModules() const { return m_availableModules; }

  private:
    void populateModules();
    MainWindowModuleVec m_availableModules;
};

#define REGISTER_MAIN_WINDOW_MODULE(module)                                                        \
    m_availableModules.push_back(std::unique_ptr<IMainWindowModule>(new module));
