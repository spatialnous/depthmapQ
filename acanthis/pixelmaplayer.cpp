// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "pixelmaplayer.h"

#include "agl/composite/aglpixelmap.h"

PixelMapLayer::PixelMapLayer(PointMap &map)
    : m_pointMap(map), MapLayer(QString::fromStdString(map.getName()), map.getAttributeTable()) {}

std::unique_ptr<AGLMap> PixelMapLayer::constructGLMap() {
    return std::unique_ptr<AGLPixelMap>(std::unique_ptr<AGLPixelMap>(new AGLPixelMap(m_pointMap)));
};
