// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "aglmap.h"

#include "../base/agllines.h"
#include "../derived/aglpolygons.h"
#include "../derived/aglregularpolygons.h"

#include "salalib/shapemap.h"

class AGLShapeMap : public AGLMap {
  public:
    AGLShapeMap(ShapeMap &shapeMap, unsigned int pointSides, float pointRadius)
        : AGLMap(), m_pointSides(pointSides), m_pointRadius(pointRadius), m_shapeMap(shapeMap){};

    void initializeGL(bool m_core) override {
        m_lines.initializeGL(m_core);
        m_polygons.initializeGL(m_core);
        m_points.initializeGL(m_core);
        m_hoveredShapes.initializeGL(m_core);
    }

    void updateGL(bool m_core) override {
        if (!m_datasetChanged)
            return;
        if (m_forceReloadGLObjects) {
            loadGLObjects();
            m_forceReloadGLObjects = false;
        }
        m_lines.updateGL(m_core);
        m_polygons.updateGL(m_core);
        m_points.updateGL(m_core);
        m_datasetChanged = false;
    }

    void updateHoverGL(bool m_core) override {
        if (m_hoverStoreInvalid) {
            m_hoveredShapes.updateGL(m_core);
            m_hoverStoreInvalid = false;
        }
    }

    void cleanup() override {
        m_lines.cleanup();
        m_polygons.cleanup();
        m_points.cleanup();
        m_hoveredShapes.cleanup();
    }

    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView,
                 const QMatrix4x4 &m_mModel) override {
        m_lines.paintGL(m_mProj, m_mView, m_mModel);
        m_polygons.paintGL(m_mProj, m_mView, m_mModel);
        m_points.paintGL(m_mProj, m_mView, m_mModel);
        glLineWidth(10);
        m_hoveredShapes.paintGL(m_mProj, m_mView, m_mModel);
        glLineWidth(1);
    }

    void loadGLObjects() override;
    void loadGLObjectsRequiringGLContext() override{};
    void highlightHoveredItems(const QtRegion &region) override { highlightHoveredShapes(region); };

    void highlightHoveredShapes(const QtRegion &region);

  protected:
    AGLLines m_lines;
    AGLPolygons m_polygons;
    AGLRegularPolygons m_points;
    AGLLines m_hoveredShapes;
    const unsigned int m_pointSides;
    const float m_pointRadius;

  private:
    ShapeMap &m_shapeMap;
};
