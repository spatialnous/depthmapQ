// SPDX-FileCopyrightText: 2018 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "../base/agltriangles.h"

#include "salalib/pafcolor.h"

#include "genlib/p2dpoly.h"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>
#include <qopengl.h>

#include <memory>

/**
 * @brief Meant to represent regular polygons i.e. those that are equiangular (all angles
 * are equal in measure) and equilateral (all sides have the same length). Ideal for
 * representing sets of disks/stars/squares etc.
 */
class AGLRegularPolygons : public AGLTriangles {
  public:
    void loadPolygonData(const std::vector<std::pair<Point2f, PafColor>> &colouredPoints,
                         const int sides, const float radius);
};
