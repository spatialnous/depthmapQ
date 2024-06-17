// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "aglobject.h"

#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>
#include <qopengl.h>

class AGLRasterTexture : public AGLObject {
  public:
    AGLRasterTexture();
    void loadRegionData(float minX, float minY, float maxX, float maxY);
    void loadPixelData(QImage &data);
    void paintGL(const QMatrix4x4 &m_proj, const QMatrix4x4 &m_camera,
                 const QMatrix4x4 &m_mModel) override;
    void initializeGL(bool coreProfile) override;
    void updateGL(bool coreProfile) override;
    void cleanup() override;
    int vertexCount() const { return m_count / DATA_DIMENSIONS; }
    AGLRasterTexture(const AGLRasterTexture &) = delete;
    AGLRasterTexture &operator=(const AGLRasterTexture &) = delete;

  private:
    int DATA_DIMENSIONS = 5;

    void setupVertexAttribs();
    const GLfloat *constData() const { return m_data.constData(); }
    void add(const QVector3D &v, const QVector2D &tc);

    QVector<GLfloat> m_data;
    int m_count;
    bool m_built = false;

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_textureSamplerLoc;

    QOpenGLTexture m_texture;
};
