// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "genlib/p2dpoly.h"
#include "salalib/pafcolor.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QRgb>
#include <QVector3D>
#include <QVector>
#include <qopengl.h>

class GLLines {
    friend class testgllines;

  public:
    GLLines();
    void loadLineData(const std::vector<std::pair<SimpleLine, PafColor>> &colouredLines);
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel);
    void initializeGL(bool coreProfile);
    void updateGL(bool coreProfile);
    void cleanup();
    int vertexCount() const { return m_count / DATA_DIMENSIONS; }
    GLLines(const GLLines &) = delete;
    GLLines &operator=(const GLLines &) = delete;

  private:
    const int DATA_DIMENSIONS = 6;
    void setupVertexAttribs();
    const GLfloat *constData() const { return m_data.constData(); }
    void add(const QVector3D &v, const QVector3D &c);

    QVector<GLfloat> m_data;
    int m_count;
    bool m_built = false;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
};
