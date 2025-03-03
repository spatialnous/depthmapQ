// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "gldynamicrect.h"

class GLDynamicLine : public GLDynamicRect {
  public:
    GLDynamicLine();
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel,
                 const QMatrix2x2 &m_selectionBounds);
};
