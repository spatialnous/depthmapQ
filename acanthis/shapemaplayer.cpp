// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "shapemaplayer.h"

#include "agl/composite/aglshapemap.h"

ShapeMapLayer::ShapeMapLayer(ShapeMap &map)
    : MapLayer(QString::fromStdString(map.getName()), map.getAttributeTable()), m_shapeMap(map) {}

std::unique_ptr<AGLMap> ShapeMapLayer::constructGLMap() {
    return std::unique_ptr<AGLShapeMap>(std::unique_ptr<AGLShapeMap>(
        new AGLShapeMap(m_shapeMap, 8, static_cast<float>(m_shapeMap.getSpacing()) * 0.1f)));
};
