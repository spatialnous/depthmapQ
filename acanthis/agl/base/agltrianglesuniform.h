// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "aglobject.h"

#include "genlib/p2dpoly.h"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QRgb>
#include <QVector3D>
#include <QVector>

/**
 * @brief General triangles representation. All same colour
 */

class AGLTrianglesUniform : public AGLObject {
  public:
    AGLTrianglesUniform();
    void loadTriangleData(const std::vector<Point2f> &points, const QRgb &polyColour);
    void paintGL(const QMatrix4x4 &mProj, const QMatrix4x4 &mView,
                 const QMatrix4x4 &mModel) override;
    void initializeGL(bool core) override;
    void updateGL(bool core) override;
    void cleanup() override;
    void updateColour(const QRgb &polyColour);
    int vertexCount() const { return m_count / DATA_DIMENSIONS; }
    AGLTrianglesUniform(const AGLTrianglesUniform &) = delete;
    AGLTrianglesUniform &operator=(const AGLTrianglesUniform &) = delete;

  private:
    const int DATA_DIMENSIONS = 3;
    void setupVertexAttribs();
    const GLfloat *constData() const { return m_data.constData(); }
    void add(const QVector3D &v);

    QVector<GLfloat> m_data;
    int m_count;
    bool m_built = false;
    QVector4D m_colour = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_colourVectorLoc;
};
