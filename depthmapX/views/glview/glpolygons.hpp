// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "gltrianglesuniform.h"

#include "salalib/pafcolor.h"

#include "salalib/genlib/p2dpoly.h"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>

#include <memory>

/**
 * @brief The GLPolygons class is a plain wrapper class for multiple GLPolygon
 * that acts as if it's a single globject
 */
class GLPolygons {
  public:
    void
    loadPolygonData(const std::vector<std::pair<std::vector<Point2f>, PafColor>> &colouredPolygons);
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel);
    void initializeGL(bool m_core);
    void updateGL(bool m_core);
    void cleanup();

  private:
    std::vector<std::unique_ptr<GLTrianglesUniform>> m_polygons;
};
