// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "dminterface/shapemapdm.hpp"
#include "gllines.hpp"
#include "glpolygons.hpp"
#include "glregularpolygons.hpp"

class GLShapeMap {
  public:
    void initializeGL(bool m_core) {
        m_lines.initializeGL(m_core);
        m_polygons.initializeGL(m_core);
        m_points.initializeGL(m_core);
    }
    void updateGL(bool m_core) {
        m_lines.updateGL(m_core);
        m_polygons.updateGL(m_core);
        m_points.updateGL(m_core);
    }
    void cleanup() {
        m_lines.cleanup();
        m_polygons.cleanup();
        m_points.cleanup();
    }
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel) {
        m_lines.paintGL(m_mProj, m_mView, m_mModel);
        m_polygons.paintGL(m_mProj, m_mView, m_mModel);
        m_points.paintGL(m_mProj, m_mView, m_mModel);
    }
    void
    loadGLObjects(const std::vector<std::pair<SimpleLine, PafColor>> &colouredLines,
                  const std::vector<std::pair<std::vector<Point2f>, PafColor>> &colouredPolygons,
                  const std::vector<std::pair<Point2f, PafColor>> &colouredPoints,
                  const int pointSides, const float pointRadius);
    void loadGLObjects(ShapeMapDM &shapeMap);

  private:
    GLLines m_lines;
    GLPolygons m_polygons;
    GLRegularPolygons m_points;
};
