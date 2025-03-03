// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "glshapemap.hpp"

void GLShapeMap::loadGLObjects(
    const std::vector<std::pair<SimpleLine, PafColor>> &colouredLines,
    const std::vector<std::pair<std::vector<Point2f>, PafColor>> &colouredPolygons,
    const std::vector<std::pair<Point2f, PafColor>> &colouredPoints, const int pointSides,
    const float pointRadius) {
    m_lines.loadLineData(colouredLines);
    m_polygons.loadPolygonData(colouredPolygons);
    m_points.loadPolygonData(colouredPoints, pointSides, pointRadius);
}

void GLShapeMap::loadGLObjects(ShapeMapDX &shapeMap) {
    loadGLObjects(shapeMap.getAllLinesWithColour(), shapeMap.getAllPolygonsWithColour(),
                  shapeMap.getAllPointsWithColour(), 8, shapeMap.getSpacing() * 0.1);
}
