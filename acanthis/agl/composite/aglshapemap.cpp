// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "aglshapemap.h"

void AGLShapeMap::loadGLObjects() {
    m_lines.loadLineData(m_shapeMap.getAllLinesWithColour());
    m_polygons.loadPolygonData(m_shapeMap.getAllPolygonsWithColour());
    m_points.loadPolygonData(m_shapeMap.getAllPointsWithColour(), m_pointSides, m_pointRadius);
}

void AGLShapeMap::highlightHoveredShapes(const QtRegion &region) {

    auto shapesInRegion = m_shapeMap.getShapesInRegion(region);
    if (!shapesInRegion.empty()) {
        std::vector<std::pair<SimpleLine, PafColor>> colouredLines;
        std::vector<std::pair<std::vector<Point2f>, PafColor>> colouredPolygons;
        std::vector<std::pair<Point2f, PafColor>> colouredPoints;
        for (auto keyShape : shapesInRegion) {
            AttributeKey key = AttributeKey(keyShape.first);
            const SalaShape &shape = keyShape.second;

            const AttributeRow &row = m_shapeMap.getAttributeTable().getRow(key);
            PafColor colour =
                dXreimpl::getDisplayColor(key, row, m_shapeMap.getAttributeTableHandle(), true);

            if (shape.isLine()) {
                colouredLines.push_back(std::make_pair(SimpleLine(shape.getLine()), colour));
            } else if (shape.isPolyLine()) {
                for (size_t n = 0; n < shape.m_points.size() - 1; n++) {
                    colouredLines.push_back(std::make_pair(
                        SimpleLine(shape.m_points[n], shape.m_points[n + 1]), colour));
                }
            } else if (shape.isPolygon()) {
                for (size_t n = 0; n < shape.m_points.size() - 1; n++) {
                    colouredLines.push_back(std::make_pair(
                        SimpleLine(shape.m_points[n], shape.m_points[n + 1]), PafColor(1, 1, 0)));
                }
                colouredLines.push_back(std::make_pair(
                    SimpleLine(shape.m_points.back(), shape.m_points.front()), PafColor(1, 1, 0)));
            } else {
                if (shape.isPoint()) {
                    colouredPoints.push_back(
                        std::make_pair(shape.getCentroid(), PafColor(1, 0, 0)));
                }
            }
        }
        m_hoveredShapes.loadLineData(colouredLines);
        m_hoverStoreInvalid = true;
        m_hoverHasShapes = true;
    } else if (m_hoverHasShapes) {
        m_hoveredShapes.loadLineData(std::vector<std::pair<SimpleLine, PafColor>>());
        m_hoverStoreInvalid = true;
        m_hoverHasShapes = false;
    }

    if (m_hoverStoreInvalid) {
        //        update();
    }
}
