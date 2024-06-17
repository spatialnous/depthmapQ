// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "../base/aglobject.h"

class AGLObjects : public AGLObject {

  protected:
    bool m_datasetChanged = false;
    bool m_forceReloadGLObjects = false;

  public:
    virtual void loadGLObjects() = 0;
    virtual void loadGLObjectsRequiringGLContext() = 0;
    void forceReloadGLObjects() { m_forceReloadGLObjects = true; }
};
