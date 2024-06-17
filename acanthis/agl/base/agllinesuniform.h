// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "aglobject.h"

#include "genlib/p2dpoly.h"

#include <QColor>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>
#include <qopengl.h>

class AGLLinesUniform : public AGLObject {
  public:
    AGLLinesUniform();
    void loadLineData(const std::vector<SimpleLine> &lines, const QColor &lineColour);
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView,
                 const QMatrix4x4 &m_mModel) override;
    void initializeGL(bool core) override;
    void updateGL(bool core) override;
    void cleanup() override;
    void updateColour(const QColor &lineColour);
    int vertexCount() const { return m_count / DATA_DIMENSIONS; }
    AGLLinesUniform(const AGLLinesUniform &) = delete;
    AGLLinesUniform &operator=(const AGLLinesUniform &) = delete;

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
