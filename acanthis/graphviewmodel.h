// SPDX-FileCopyrightText: 2023 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "graphmodel.h"

#include "graphviewmodel.h"
#include "pixelmaplayer.h"
#include "shapegraphlayer.h"
#include "shapemaplayer.h"

#include <QObject>
#include <qqml.h>

// This class is meant to hold display information about one view[port]
// of the graph document, such as which items are displayed

class GraphViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(
        GraphModel *graphModel WRITE setGraphModel MEMBER m_graphModel NOTIFY graphModelChanged)
    Q_PROPERTY(QString id MEMBER m_id NOTIFY idChanged)

    GraphModel *m_graphModel;
    QString m_id;

    // These are stored as QSharedPointers so that they can be used by the map view tree
    // which stores everything in QSharedPointers
    QList<QSharedPointer<MapLayer>> m_mapLayers;

  public:
    Q_INVOKABLE explicit GraphViewModel(QString id, QObject *parent = nullptr) : m_id(id){};
    QList<QSharedPointer<MapLayer>> &getMapLayers() { return m_mapLayers; }
    const QList<QSharedPointer<MapLayer>> &getMapLayers() const { return m_mapLayers; }
    bool hasMetaGraph() const { return m_graphModel->hasMetaGraph(); }
    const QtRegion getBoundingBox() const { return m_graphModel->getMetaGraph().getBoundingBox(); }

    void setGraphModel(GraphModel *graphModel) {
        if (graphModel == nullptr)
            return;
        m_graphModel = graphModel;

        for (ShapeMap &shapeMap : graphModel->getMetaGraph().getDataMaps()) {
            m_mapLayers.append(QSharedPointer<ShapeMapLayer>(new ShapeMapLayer(shapeMap)));
        }
        for (auto &drawingFile : graphModel->getMetaGraph().m_drawingFiles) {
            for (ShapeMap &shapeMap : drawingFile.m_spacePixels) {
                m_mapLayers.append(QSharedPointer<ShapeMapLayer>(new ShapeMapLayer(shapeMap)));
            }
        }
        for (auto &shapeGraph : graphModel->getMetaGraph().getShapeGraphs()) {
            m_mapLayers.append(QSharedPointer<ShapeGraphLayer>(new ShapeGraphLayer(*shapeGraph)));
        }
        for (PointMap &pointMap : graphModel->getMetaGraph().getPointMaps()) {
            m_mapLayers.append(QSharedPointer<PixelMapLayer>(new PixelMapLayer(pointMap)));
        }

        emit graphModelChanged();
    }

  signals:
    void graphModelChanged();
    void idChanged();
};
