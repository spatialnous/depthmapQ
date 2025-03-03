// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "viewhelpers.hpp"

#include <time.h>

namespace ViewHelpers {
    Point2f calculateCenter(const QPointF &point, const QPointF &oldCentre, double factor) {
        int diffX = static_cast<int>(oldCentre.x() - point.x());
        int diffY = static_cast<int>(oldCentre.y() - point.y());
        return Point2f(point.x() + double(diffX) * factor, point.y() + double(diffY) * factor);
    }

    std::string getCurrentDate() {
        time_t now = time(NULL);
        char timeString[11];
        strftime(timeString, 11, "%Y/%m/%d", localtime(&now));
        return timeString;
    }

} // namespace ViewHelpers
