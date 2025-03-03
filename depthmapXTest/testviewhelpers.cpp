// SPDX-FileCopyrightText: 2017 Christian Sailer
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "depthmapX/views/viewhelpers.hpp"

#include "catch_amalgamated.hpp"

#include <iomanip>
#include <sstream>
#include <time.h>

TEST_CASE("Calculating the new center", "[calculateCenter]") {
    const auto point = QPointF(100, 100);
    const auto oldCenter = QPointF(200, 200);
    auto newCenter = Point2f(150, 150);

    REQUIRE(ViewHelpers::calculateCenter(point, oldCenter, 0.5) == newCenter);

    newCenter.x = 300;
    newCenter.y = 300;

    REQUIRE(ViewHelpers::calculateCenter(point, oldCenter, 2.0) == newCenter);
}

TEST_CASE("Date string format", "[getCurrentDate]") {
    auto now = time(NULL);
    const tm *ltime = localtime(&now);
    std::stringstream sstream;
    sstream << ltime->tm_year + 1900 << "/" << std::setfill('0') << std::setw(2)
            << ltime->tm_mon + 1 << "/" << std::setfill('0') << std::setw(2) << ltime->tm_mday
            << std::flush;

    REQUIRE(ViewHelpers::getCurrentDate() == sstream.str().c_str());
}
