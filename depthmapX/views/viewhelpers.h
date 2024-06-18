// SPDX-FileCopyrightText: 2017 Christian Sailer
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "genlib/p2dpoly.h"
#include <QPoint>
#include <string>

namespace ViewHelpers {
    Point2f calculateCenter(const QPointF &point, const QPointF &oldCentre, double factor);
    std::string getCurrentDate();
} // namespace ViewHelpers
