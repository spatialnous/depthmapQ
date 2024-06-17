// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "aglobject.h"

#include <QColor>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QVector3D>
#include <QVector>
#include <qopengl.h>

class AGLDynamicRect : public AGLObject {
  public:
    AGLDynamicRect();

    void paintGL(const QMatrix4x4 &mProj, const QMatrix4x4 &mView,
                 const QMatrix4x4 &mModel) override;
    void initializeGL(bool core) override;
    void updateGL(bool core) override;
    void cleanup() override;

    void setFillColour(const QColor &colour);
    void setStrokeColour(const QColor &colour);
    int vertexCount() const { return m_count / DATA_DIMENSIONS; }
    void setSelectionBounds(QMatrix2x2 selectionBounds) {
        m_selectionBounds = std::move(selectionBounds);
    }

    AGLDynamicRect(const AGLDynamicRect &) = delete;
    AGLDynamicRect &operator=(const AGLDynamicRect &) = delete;

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

    QMatrix2x2 m_selectionBounds;

  private:
    const int DATA_DIMENSIONS = 1;
    void setupVertexAttribs();
    int count() const { return m_count; }
    const GLfloat *constData() const { return m_data.constData(); }
};
