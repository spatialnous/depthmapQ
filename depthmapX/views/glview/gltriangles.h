// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "genlib/p2dpoly.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QRgb>
#include <QVector3D>
#include <QVector>
#include <qopengl.h>

/**
 * @brief General triangles representation. Each triangle may have its own colour
 */

class GLTriangles {
  public:
    GLTriangles() : m_count(0), m_program(0) {}
    void loadTriangleData(const std::vector<std::pair<std::vector<Point2f>, QRgb>> &triangleData);
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel);
    void initializeGL(bool m_core);
    void updateGL(bool m_core);
    void cleanup();
    void updateColour(const QRgb &polyColour);
    int vertexCount() const { return m_count / DATA_DIMENSIONS; }
    GLTriangles(const GLTriangles &) = delete;
    GLTriangles &operator=(const GLTriangles &) = delete;

  protected:
    void init(int numTriangles) {
        m_built = false;
        m_count = 0;
        m_data.resize(numTriangles * 3 * DATA_DIMENSIONS);
    }
    void add(const QVector3D &v, const QVector3D &c);

  private:
    const int DATA_DIMENSIONS = 6;
    void setupVertexAttribs();
    const GLfloat *constData() const { return m_data.constData(); }

    QVector<GLfloat> m_data;
    int m_count;
    bool m_built = false;
    QVector4D m_colour = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
};
