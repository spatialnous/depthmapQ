// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "depthmapX/views/glview/gllines.h"

#include "catch_amalgamated.hpp"

TEST_CASE("Test GLLines::loadLineData()", "") {
    Point2f line1Start(0, 0);
    Point2f line1End(2, 4);
    PafColor line1colour(255, 0, 0);
    Point2f line2Start(1, 1);
    Point2f line2End(3, 5);
    PafColor line2colour(0, 255, 0);

    std::vector<std::pair<SimpleLine, PafColor>> colouredLines;

    std::pair<SimpleLine, PafColor> colouredLine1 =
        std::pair<SimpleLine, PafColor>(SimpleLine(line1Start, line1End), line1colour);
    colouredLines.push_back(colouredLine1);

    std::pair<SimpleLine, PafColor> colouredLine2 =
        std::pair<SimpleLine, PafColor>(SimpleLine(line2Start, line2End), line2colour);
    colouredLines.push_back(colouredLine2);

    GLLines gllines;
    gllines.loadLineData(colouredLines);

    REQUIRE(gllines.vertexCount() == 4);
}
