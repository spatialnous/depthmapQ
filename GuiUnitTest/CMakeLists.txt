# SPDX-FileCopyrightText: 2017 Christian Sailer
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(GuiUnitTest GuiUnitTest)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Find the QtWidgets library
find_package(Qt6 COMPONENTS Core Widgets Gui OpenGL REQUIRED)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)

set(guiUnitTest_SRCS
    main.cpp
    selfcleaningfile.h
    testviewhelpers.cpp 
    testsettings.cpp 
    ../depthmapX/settingsimpl.cpp 
    ../depthmapX/views/viewhelpers.cpp)

include_directories("../ThirdParty/Catch" "../ThirdParty/FakeIt" "../depthmapX")

set(LINK_LIBS salalib genlib Qt6::Core)

add_executable(${GuiUnitTest} ${guiUnitTest_SRCS})

target_compile_options(${GuiUnitTest} PRIVATE ${COMPILE_WARNINGS})

target_link_libraries(${GuiUnitTest} ${LINK_LIBS})

