// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "../composite/aglgraph.h"
#include "aglshapemap.h"

#include "salalib/shapegraph.h"

class AGLShapeGraph : public AGLShapeMap {
  public:
    AGLShapeGraph(ShapeGraph &shapeGraph, int pointSides, float pointRadius)
        : AGLShapeMap(shapeGraph, pointSides, pointRadius), m_shapeGraph(shapeGraph){};

    void initializeGL(bool core) override {
        AGLShapeMap::initializeGL(core);
        m_glGraph.initializeGL(core);
    }

    void updateGL(bool core) override {
        if (!m_datasetChanged)
            return;
        if (m_forceReloadGLObjects) {
            loadGLObjects();
            AGLShapeMap::loadGLObjects();
            m_glGraph.loadGLObjects();
            m_forceReloadGLObjects = false;
        }
        AGLShapeMap::updateGL(core);
        m_glGraph.updateGL(core);
        m_datasetChanged = false;
    }

    void cleanup() override {
        AGLShapeMap::cleanup();
        m_glGraph.cleanup();
    }

    void paintGL(const QMatrix4x4 &mProj, const QMatrix4x4 &mView,
                 const QMatrix4x4 &mModel) override {
        AGLShapeMap::paintGL(mProj, mView, mModel);
        if (m_showLinks) {
            m_glGraph.paintGL(mProj, mView, mModel);
        }
    }

    void showLinks(bool showLinks) { m_showLinks = showLinks; }
    void loadGLObjects() override;

  private:
    ShapeGraph &m_shapeGraph;

    AGLGraph m_glGraph;

    bool m_showLinks = true;
};
