// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "gllinesuniform.h"
#include "glshapemap.h"

#include "salalib/shapegraph.h"
#include "salaobj/shapegraphdx.h"

class GLShapeGraph {
  public:
    void initializeGL(bool m_core) {
        m_shapeMap.initializeGL(m_core);
        m_linkLines.initializeGL(m_core);
        m_linkFills.initializeGL(m_core);
        m_unlinkFills.initializeGL(m_core);
        m_unlinkLines.initializeGL(m_core);
    }
    void updateGL(bool m_core) {
        m_shapeMap.updateGL(m_core);
        m_linkLines.updateGL(m_core);
        m_linkFills.updateGL(m_core);
        m_unlinkFills.updateGL(m_core);
        m_unlinkLines.updateGL(m_core);
    }
    void cleanup() {
        m_shapeMap.cleanup();
        m_linkLines.cleanup();
        m_linkFills.cleanup();
        m_unlinkFills.cleanup();
        m_unlinkLines.cleanup();
    }
    void paintGLOverlay(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView,
                        const QMatrix4x4 &m_mModel) {
        if (m_showLinks) {
            QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
            glFuncs->glLineWidth(3);
            m_linkLines.paintGL(m_mProj, m_mView, m_mModel);
            m_linkFills.paintGL(m_mProj, m_mView, m_mModel);
            m_unlinkLines.paintGL(m_mProj, m_mView, m_mModel);
            m_unlinkFills.paintGL(m_mProj, m_mView, m_mModel);
            glFuncs->glLineWidth(1);
        }
    }
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel) {
        m_shapeMap.paintGL(m_mProj, m_mView, m_mModel);
    }
    void showLinks(bool showLinks) { m_showLinks = showLinks; }
    void loadGLObjects(ShapeGraphDX &shapeGraph);

  private:
    GLShapeMap m_shapeMap;
    GLLinesUniform m_linkLines;
    GLTrianglesUniform m_linkFills;
    GLLinesUniform m_unlinkLines;
    GLTrianglesUniform m_unlinkFills;

    bool m_showLinks = false;
};
