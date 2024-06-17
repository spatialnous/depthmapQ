// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "maplayer.h"

#include "salalib/shapegraph.h"

class ShapeGraphLayer : public MapLayer {
    ShapeGraph &m_shapeGraph;

  public:
    explicit ShapeGraphLayer(ShapeGraph &map);

    std::unique_ptr<AGLMap> constructGLMap() override;

    bool hasGraph() override { return true; }
};
