// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "aglviewmodel.h"

#include "../composite/aglmap.h"

#include "maplayer.h"

class AGLMapViewModel : public AGLViewModel {
    AGLMap &getGLMap(MapLayer *mapLayer);
    std::map<MapLayer *, std::unique_ptr<AGLMap>> m_glMaps;

  public:
    using AGLViewModel::AGLViewModel;
    const QList<QSharedPointer<MapLayer>> &getMaps() const;
    void cleanup() override;
    void loadGLObjects() override;
    void initializeGL(bool m_core) override;
    void loadGLObjectsRequiringGLContext() override;
    void updateGL(bool m_core) override;
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView,
                 const QMatrix4x4 &m_mModel) override;

    void highlightHoveredItems(const QtRegion &region);
};
