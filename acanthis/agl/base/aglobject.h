// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

// Interface meant

#pragma once

#include <QMatrix4x4>

class AGLObject {
  public:
    virtual void initializeGL(bool m_core) = 0;
    virtual void updateGL(bool m_core) = 0;
    virtual void cleanup() = 0;
    virtual void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView,
                         const QMatrix4x4 &m_mModel) = 0;
    virtual ~AGLObject(){};
};
