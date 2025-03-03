// SPDX-FileCopyrightText: 2018 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "gltriangles.hpp"

#include "salalib/pafcolor.hpp"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>
#include <memory>

/**
 * @brief Meant to represent regular polygons i.e. those that are equiangular (all angles are equal
 * in measure) and equilateral (all sides have the same length). Ideal for representing sets of
 * disks/stars/squares etc.
 */
class GLRegularPolygons : public GLTriangles {
  public:
    void loadPolygonData(const std::vector<std::pair<Point2f, PafColor>> &colouredPoints,
                         const int sides, const float radius);
};
