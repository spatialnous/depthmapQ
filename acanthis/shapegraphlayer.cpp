// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "shapegraphlayer.h"

#include "agl/composite/aglshapegraph.h"

ShapeGraphLayer::ShapeGraphLayer(ShapeGraph &map)
    : MapLayer(QString::fromStdString(map.getName()), map.getAttributeTable()), m_shapeGraph(map) {}

std::unique_ptr<AGLMap> ShapeGraphLayer::constructGLMap() {
    return std::unique_ptr<AGLShapeGraph>(
        new AGLShapeGraph(m_shapeGraph, 8, static_cast<float>(m_shapeGraph.getSpacing()) * 0.1f));
};
