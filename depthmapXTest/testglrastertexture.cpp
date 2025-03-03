// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "depthmapX/views/glview/glrastertexture.hpp"

#include "catch_amalgamated.hpp"

TEST_CASE("Test GLRasterTexture::loadRegionData()", "") {
    float bottomLeftX = 0;
    float bottomLeftY = 0;
    float topRightX = 2;
    float topRightY = 4;

    GLRasterTexture glrastertexture;
    glrastertexture.loadRegionData(bottomLeftX, bottomLeftY, topRightX, topRightY);

    REQUIRE(glrastertexture.vertexCount() == 4);
}
