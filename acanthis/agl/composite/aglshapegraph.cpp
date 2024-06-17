// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "aglshapegraph.h"

void AGLShapeGraph::loadGLObjects() {
    AGLShapeMap::loadGLObjects();
    m_glGraph.setNodeSize(m_shapeGraph.getSpacing() * 0.05);
    m_glGraph.setGraphCornerRadius(m_shapeGraph.getSpacing() * 0.3);

    std::vector<Connector> &connections = m_shapeGraph.getConnections();
    auto &shapes = m_shapeGraph.getAllShapes();
    auto shapeIter = shapes.begin();
    for (Connector &connector : connections) {
        auto &fromShape = shapeIter->second;
        auto fromCentroid = fromShape.getCentroid();
        for (size_t &connection : connector.m_connections) {
            auto &toShape = shapes[connection];
            auto toCentroid = toShape.getCentroid();
            m_glGraph.addConnection(
                SimpleLine(fromCentroid.x, fromCentroid.y, toCentroid.x, toCentroid.y),
                intersection_point(fromShape.getLine(), toShape.getLine()));
        }
        for (auto &secConn : connector.m_forward_segconns) {
            auto &toShape = shapes[secConn.first.ref];
            auto toCentroid = toShape.getCentroid();
            const Point2f &intp =
                secConn.first.dir != -1 ? toShape.getLine().start() : toShape.getLine().end();
            m_glGraph.addConnection(
                SimpleLine(fromCentroid.x, fromCentroid.y, toCentroid.x, toCentroid.y), intp);
        }
        for (auto &secConn : connector.m_back_segconns) {
            auto &toShape = shapes[secConn.first.ref];
            auto toCentroid = toShape.getCentroid();
            const Point2f &intp =
                secConn.first.dir != -1 ? toShape.getLine().start() : toShape.getLine().end();
            m_glGraph.addConnection(
                SimpleLine(fromCentroid.x, fromCentroid.y, toCentroid.x, toCentroid.y), intp);
        }
        shapeIter++;
    }
    m_glGraph.setLinks(m_shapeGraph.getAllLinkLines());
    m_glGraph.setUnlinks(m_shapeGraph.getAllUnlinkPoints());
    m_glGraph.loadGLObjects();
}
