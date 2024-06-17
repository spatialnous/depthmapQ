// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "agldynamicrect.h"

class AGLDynamicLine : public AGLDynamicRect {
  public:
    AGLDynamicLine();
    void paintGL(const QMatrix4x4 &mProj, const QMatrix4x4 &mView, const QMatrix4x4 &mModel);
};
