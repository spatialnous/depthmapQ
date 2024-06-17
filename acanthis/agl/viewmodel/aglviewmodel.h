// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "../derived/aglobjects.h"

#include "graphviewmodel.h"

class AGLViewModel : public AGLObjects {
  protected:
    const GraphViewModel *m_graphViewModel = nullptr;

  public:
    AGLViewModel(const GraphViewModel *graphViewModel) : m_graphViewModel(graphViewModel) {}
    bool hasGraphViewModel() const { return m_graphViewModel != nullptr; }
};
