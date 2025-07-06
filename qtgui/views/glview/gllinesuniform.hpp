// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "salalib/genlib/simpleline.hpp"

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QRgb>
#include <QVector3D>
#include <QVector>

class GLLinesUniform {
  public:
    GLLinesUniform();
    void loadLineData(const std::vector<SimpleLine> &lines, const QRgb &lineColour);
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel);
    void initializeGL(bool coreProfile);
    void updateGL(bool coreProfile);
    void cleanup();
    void updateColour(const QRgb &lineColour);
    int vertexCount() const { return m_count / DATA_DIMENSIONS; }
    GLLinesUniform(const GLLinesUniform &) = delete;
    GLLinesUniform &operator=(const GLLinesUniform &) = delete;

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
