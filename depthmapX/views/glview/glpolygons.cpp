// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "glpolygons.hpp"

#include "glutriangulator.hpp"

/**
 * @brief GLPolygons::GLPolygons
 * This class is an OpenGL representation of multiple polygons of different colour
 */

void GLPolygons::loadPolygonData(
    const std::vector<std::pair<std::vector<Point2f>, PafColor>> &colouredPolygons) {
    m_polygons.clear();
    for (auto &colouredPolygon : colouredPolygons) {
        const std::vector<Point2f> &points = colouredPolygon.first;
        QRgb colour = qRgb(colouredPolygon.second.redb(), colouredPolygon.second.greenb(),
                           colouredPolygon.second.blueb());

        m_polygons.push_back(std::unique_ptr<GLTrianglesUniform>(new GLTrianglesUniform));

        std::vector<Point2f> triangulated = GLUTriangulator::triangulate(points);
        m_polygons.back()->loadTriangleData(triangulated, colour);
    }
}

void GLPolygons::initializeGL(bool m_core) {
    for (auto &polygon : m_polygons) {
        polygon->initializeGL(m_core);
    }
}

void GLPolygons::updateGL(bool m_core) {
    for (auto &polygon : m_polygons) {
        polygon->updateGL(m_core);
    }
}

void GLPolygons::cleanup() {
    for (auto &polygon : m_polygons) {
        polygon->cleanup();
    }
}

void GLPolygons::paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView,
                         const QMatrix4x4 &m_mModel) {
    for (auto &polygon : m_polygons) {
        polygon->paintGL(m_mProj, m_mView, m_mModel);
    }
}
