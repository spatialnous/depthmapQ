// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "salalib/genlib/point2f.hpp"

#include <vector>

// This is required to silence the warnings for OpenGL deprecation in macOS
#ifndef GL_SILENCE_DEPRECATION
#define GL_SILENCE_DEPRECATION true
#endif

class GLUTriangulator {
  public:
    static std::vector<Point2f> triangulate(const std::vector<Point2f> &polygon);
};
