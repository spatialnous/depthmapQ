// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "maplayer.h"

#include "salalib/pointdata.h"

class PixelMapLayer : public MapLayer {

    PointMap &m_pointMap;

  public:
    explicit PixelMapLayer(PointMap &map);

    std::unique_ptr<AGLMap> constructGLMap() override;

    bool hasGraph() override { return m_pointMap.isProcessed(); }
};
