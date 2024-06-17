// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "shapemaplayer.h"

#include "agl/composite/aglshapemap.h"

ShapeMapLayer::ShapeMapLayer(ShapeMap &map)
    : m_shapeMap(map), MapLayer(QString::fromStdString(map.getName()), map.getAttributeTable()) {}

std::unique_ptr<AGLMap> ShapeMapLayer::constructGLMap() {
    return std::unique_ptr<AGLShapeMap>(std::unique_ptr<AGLShapeMap>(
        new AGLShapeMap(m_shapeMap, 8, m_shapeMap.getSpacing() * 0.1)));
};
