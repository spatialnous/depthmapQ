// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "glshapegraph.hpp"

#include "salalib/geometrygenerators.hpp"

void GLShapeGraph::loadGLObjects(ShapeGraphDX &shapeGraph) {
    m_shapeMap.loadGLObjects(shapeGraph);
    const std::vector<SimpleLine> &linkLines = shapeGraph.getAllLinkLines();
    std::vector<Point2f> linkPointLocations;
    for (auto &linkLine : linkLines) {
        linkPointLocations.push_back(linkLine.start());
        linkPointLocations.push_back(linkLine.end());
    }

    const std::vector<Point2f> &linkFillTriangles =
        GeometryGenerators::generateMultipleDiskTriangles(32, shapeGraph.getSpacing() * 0.1,
                                                          linkPointLocations);
    m_linkFills.loadTriangleData(linkFillTriangles, qRgb(0, 0, 0));

    std::vector<SimpleLine> linkFillPerimeters = GeometryGenerators::generateMultipleCircleLines(
        32, shapeGraph.getSpacing() * 0.1, linkPointLocations);
    linkFillPerimeters.insert(linkFillPerimeters.end(), linkLines.begin(), linkLines.end());
    m_linkLines.loadLineData(linkFillPerimeters, qRgb(0, 255, 0));

    const std::vector<Point2f> &unlinkPoints = shapeGraph.getAllUnlinkPoints();

    const std::vector<Point2f> &unlinkFillTriangles =
        GeometryGenerators::generateMultipleDiskTriangles(32, shapeGraph.getSpacing() * 0.1,
                                                          unlinkPoints);
    m_unlinkFills.loadTriangleData(unlinkFillTriangles, qRgb(255, 255, 255));

    const std::vector<SimpleLine> &unlinkFillPerimeters =
        GeometryGenerators::generateMultipleCircleLines(32, shapeGraph.getSpacing() * 0.1,
                                                        unlinkPoints);
    m_unlinkLines.loadLineData(unlinkFillPerimeters, qRgb(255, 0, 0));
}
