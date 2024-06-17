# SPDX-FileCopyrightText: 2020 Petros Koutsolampros
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(segmentpathsclitest segmentpathsclitest)
set(segmentpathsclitest_SRCS
    segmentshortestpathparsertest.cpp)

set(modules_cliTest "${modules_cliTest}" "segmentpathsclitest" CACHE INTERNAL "modules_cliTest" FORCE)

add_compile_definitions(SEGMENTPATHS_CLI_TEST_LIBRARY)

add_library(${segmentpathsclitest} OBJECT ${segmentpathsclitest_SRCS})
