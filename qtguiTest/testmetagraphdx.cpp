// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "qtgui/dminterface/metagraphdm.hpp"

#include "catch_amalgamated.hpp"

TEST_CASE("Test latticeMaps", "") {
    std::unique_ptr<MetaGraphDM> mgraph(new MetaGraphDM());
    auto latticeMapIdx = mgraph->addNewLatticeMap("Kenny");
    REQUIRE(mgraph->getLatticeMaps().size() == 1);
    REQUIRE(latticeMapIdx == 0);
    REQUIRE(mgraph->getLatticeMaps()[0].getName() == "Kenny");
    REQUIRE(mgraph->getDisplayedLatticeMapRef() == latticeMapIdx);
    REQUIRE(mgraph->getDisplayedLatticeMap().getName() == "Kenny");

    SECTION("Add another and remove the first through the MetaGraph") {
        auto latticeMapIdx = mgraph->addNewLatticeMap("Stan");
        REQUIRE(mgraph->getLatticeMaps().size() == 2);
        REQUIRE(latticeMapIdx == 1);
        REQUIRE(mgraph->getLatticeMaps()[1].getName() == "Stan");
        REQUIRE(mgraph->getDisplayedLatticeMapRef() == 1);
        REQUIRE(mgraph->getDisplayedLatticeMap().getName() == "Stan");

        mgraph->setState(MetaGraphDM::DX_LATTICEMAPS);
        mgraph->setViewClass(MetaGraphDM::DX_SHOWVGATOP);
        mgraph->setDisplayedLatticeMapRef(0);
        REQUIRE(mgraph->getDisplayedLatticeMapRef() == 0);
        REQUIRE(mgraph->getDisplayedLatticeMap().getName() == "Kenny");

        mgraph->removeDisplayedMap();
        REQUIRE(mgraph->getLatticeMaps().size() == 1);
        REQUIRE(mgraph->getLatticeMaps()[0].getName() == "Stan");
        REQUIRE(mgraph->getDisplayedLatticeMapRef() == 0);
    }
}
