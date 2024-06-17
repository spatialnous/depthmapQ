// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "maplayer.h"

#include "salalib/shapemap.h"

class ShapeMapLayer : public MapLayer {
    ShapeMap &m_shapeMap;

  public:
    explicit ShapeMapLayer(ShapeMap &map);

    std::unique_ptr<AGLMap> constructGLMap() override;
};
