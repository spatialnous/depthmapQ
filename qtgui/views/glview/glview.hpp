// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "gldynamicline.hpp"
#include "gldynamicrect.hpp"
#include "gllines.hpp"
#include "gllinesuniform.hpp"
#include "glpointmap.hpp"
#include "glshapegraph.hpp"

#include "graphdoc.hpp"
#include "views/mapview.hpp"

#include <QMatrix4x4>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLView : public MapView, protected QOpenGLFunctions {
    Q_OBJECT

  public:
    GLView(QGraphDoc &pDoc, Settings &settings, QWidget *parent = Q_NULLPTR);
    ~GLView();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void notifyDatasetChanged() {
        m_datasetChanged = true;
        update();
    }
    void matchViewToCurrentMetaGraph();

    virtual void OnModeJoin() override;
    virtual void OnModeUnjoin() override;
    virtual void OnViewPan() override;
    virtual void OnViewZoomIn() override;
    virtual void OnViewZoomOut() override;
    virtual void OnEditFill() override;
    virtual void OnEditSemiFill() override;
    virtual void OnEditAugmentFill() override;
    virtual void OnEditPencil() override;
    virtual void OnModeIsovist() override;
    virtual void OnModeTargetedIsovist() override;
    virtual void OnEditLineTool() override;
    virtual void OnEditPolygonTool() override;
    virtual void OnModeSeedAxial() override;
    virtual void OnEditSelect() override;
    virtual void OnViewZoomsel() override;
    void OnModeStepDepth();
    virtual void postLoadFile() override;
    virtual void OnEditCopy() override;
    virtual void OnEditSave() override;
    virtual void OnViewZoomToRegion(Region4f region) override;

  protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool eventFilter(QObject *object, QEvent *e) override;
    void closeEvent(QCloseEvent *event) override;

  private:
    bool m_perspectiveView = false;
    bool m_core;
    QMatrix4x4 m_mProj;
    QMatrix4x4 m_mView;
    QMatrix4x4 m_mModel;

    QRgb m_foreground;
    QRgb m_background;
    QSize m_initialSize;

    GLDynamicRect m_selectionRect;
    GLDynamicLine m_dragLine;
    GLLines m_axes;
    GLShapeGraph m_visibleShapeGraph;
    GLLinesUniform m_visibleDrawingLines;
    GLPixelMap m_visiblePointMap;
    GLShapeMap m_visibleDataMap;

    bool m_highlightOnHover = true;
    bool m_hoverHasShapes = false;
    bool m_hoverStoreInvalid = false;
    GLShapeMap m_hoveredShapes;
    GLShapeMap m_hoveredSecondaryShapes;
    GLLinesUniform m_hoveredPixels;
    PixelRef m_lastHoverPixel = -1;

    QPoint m_mouseLastPos;
    float m_eyePosX;
    float m_eyePosY;
    float m_minZoomFactor = 1;
    float m_zoomFactor = 20;
    float m_maxZoomFactor = 200;
    GLfloat m_screenRatio;
    int m_screenWidth;
    int m_screenHeight;
    bool m_wasPanning = false;

    int m_antialiasingSamples = 0; // set this to 0 if rendering is too slow

    Point2f getWorldPoint(const QPoint &screenPoint);
    QPoint getScreenPoint(const Point2f &worldPoint);

    bool m_datasetChanged = false;

    void panBy(int dx, int dy);
    void recalcView();
    void zoomBy(float dzf, int mouseX, int mouseY);
    void resetView();

    void highlightHoveredItems(const Region4f &region);
    void highlightHoveredPixels(const PointMapDM &map, const Region4f &region);
    void highlightHoveredPixels(const PointMapDM &map, const std::set<PixelRef> &refs);
    void highlightHoveredShapes(const ShapeMapDM &map, const Region4f &region);

    void loadAxes();
    void loadDrawingGLObjects();

    enum {
        MOUSE_MODE_NONE = 0x0000,
        MOUSE_MODE_SELECT = 0x10000,
        MOUSE_MODE_PAN = 0x0101,
        MOUSE_MODE_ZOOM_IN = 0x0202,
        MOUSE_MODE_ZOOM_OUT = 0x0204,
        MOUSE_MODE_FILL_FULL = 0x0001,
        MOUSE_MODE_FILL_SEMI = 0x0002,
        MOUSE_MODE_FILL_AUGMENT = 0x0003,
        MOUSE_MODE_PENCIL = 0x0801,
        MOUSE_MODE_SEED_ISOVIST = 0x4001,
        MOUSE_MODE_SEED_TARGETED_ISOVIST = 0x4002,
        MOUSE_MODE_SEED_AXIAL = 0x0004,
        MOUSE_MODE_LINE_TOOL = 0x0008,
        MOUSE_MODE_POLYGON_TOOL = 0x0010,
        MOUSE_MODE_POINT_STEP_DEPTH = 0x5000,
        MOUSE_MODE_JOIN = 0x20001,
        MOUSE_MODE_UNJOIN = 0x20002,
        MOUSE_MODE_SECOND_POINT = 0x00400,
    };

    int m_mouseMode = MOUSE_MODE_SELECT;

    QRectF m_mouseDragRect = QRectF(0, 0, 0, 0);

    Point2f m_tempFirstPoint;
    Point2f m_tempSecondPoint;

    int m_currentlyEditingShapeRef = -1;

    Point2f m_polyStart;
    int m_polyPoints = 0;

    inline int PixelDist(QPoint a, QPoint b) {
        return (int)sqrt(
            double((b.x() - a.x()) * (b.x() - a.x()) + (b.y() - a.y()) * (b.y() - a.y())));
    }
};
