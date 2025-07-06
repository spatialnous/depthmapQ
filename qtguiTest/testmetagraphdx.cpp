// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "qtgui/dminterface/metagraphdm.hpp"

#include "catch_amalgamated.hpp"

TEST_CASE("Test pointMaps", "") {
    std::unique_ptr<MetaGraphDM> mgraph(new MetaGraphDM());
    auto pointMapIdx = mgraph->addNewPointMap("Kenny");
    REQUIRE(mgraph->getPointMaps().size() == 1);
    REQUIRE(pointMapIdx == 0);
    REQUIRE(mgraph->getPointMaps()[0].getName() == "Kenny");
    REQUIRE(mgraph->getDisplayedPointMapRef() == pointMapIdx);
    REQUIRE(mgraph->getDisplayedPointMap().getName() == "Kenny");

    SECTION("Add another and remove the first through the MetaGraph") {
        auto pointMapIdx = mgraph->addNewPointMap("Stan");
        REQUIRE(mgraph->getPointMaps().size() == 2);
        REQUIRE(pointMapIdx == 1);
        REQUIRE(mgraph->getPointMaps()[1].getName() == "Stan");
        REQUIRE(mgraph->getDisplayedPointMapRef() == 1);
        REQUIRE(mgraph->getDisplayedPointMap().getName() == "Stan");

        mgraph->setState(MetaGraphDM::DX_POINTMAPS);
        mgraph->setViewClass(MetaGraphDM::DX_SHOWVGATOP);
        mgraph->setDisplayedPointMapRef(0);
        REQUIRE(mgraph->getDisplayedPointMapRef() == 0);
        REQUIRE(mgraph->getDisplayedPointMap().getName() == "Kenny");

        mgraph->removeDisplayedMap();
        REQUIRE(mgraph->getPointMaps().size() == 1);
        REQUIRE(mgraph->getPointMaps()[0].getName() == "Stan");
        REQUIRE(mgraph->getDisplayedPointMapRef() == 0);
    }
}
