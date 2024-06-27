// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "depthmapX/views/glview/gllinesuniform.h"

#include "catch_amalgamated.hpp"

TEST_CASE("Test GLLinesUniform::loadLineData()", "") {
    Point2f line1Start(0, 0);
    Point2f line1End(2, 4);

    Point2f line2Start(1, 1);
    Point2f line2End(3, 5);

    std::vector<SimpleLine> lines;

    SimpleLine line1 = SimpleLine(line1Start, line1End);
    lines.push_back(line1);

    SimpleLine line2 = SimpleLine(line2Start, line2End);
    lines.push_back(line2);

    QRgb lineColour = qRgb(255, 0, 0);

    GLLinesUniform gllinesuniform;
    gllinesuniform.loadLineData(lines, lineColour);

    REQUIRE(gllinesuniform.vertexCount() == 4);
}
