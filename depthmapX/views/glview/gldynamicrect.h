// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QRgb>
#include <QVector3D>
#include <QVector>
#include <qopengl.h>

class GLDynamicRect {
  public:
    GLDynamicRect();
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel,
                 const QMatrix2x2 &m_selectionBounds);
    void initializeGL(bool m_core);
    void updateGL(bool m_core);
    void cleanup();
    void setFillColour(const QRgb &fillColour);
    void setStrokeColour(const QRgb &strokeColour);
    int vertexCount() const { return m_count / DATA_DIMENSIONS; }

    GLDynamicRect(const GLDynamicRect &) = delete;
    GLDynamicRect &operator=(const GLDynamicRect &) = delete;

  protected:
    void add(const GLfloat v);

    int m_count;
    bool m_built = false;
    QVector<GLfloat> m_data;

    QOpenGLBuffer m_vbo;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLShaderProgram *m_program;

    int m_diagVertices2DLoc;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_colourVectorLoc;

    QVector4D m_colour_fill = QVector4D(0.0f, 1.0f, 0.0f, 0.3f);
    QVector4D m_colour_stroke = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);

  private:
    const int DATA_DIMENSIONS = 1;
    void setupVertexAttribs();
    int count() const { return m_count; }
    const GLfloat *constData() const { return m_data.constData(); }
};
