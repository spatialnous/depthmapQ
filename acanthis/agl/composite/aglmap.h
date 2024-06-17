// SPDX-FileCopyrightText: 2021 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "../derived/aglobjects.h"

#include "genlib/p2dpoly.h"

class AGLMap : public AGLObjects {

  protected:
    bool m_hoverStoreInvalid = false;
    bool m_hoverHasShapes = false;

  public:
    virtual ~AGLMap() {}
    virtual void updateHoverGL(bool m_core) = 0;
    virtual void highlightHoveredItems(const QtRegion &region) = 0;
};
