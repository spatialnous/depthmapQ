# SPDX-FileCopyrightText: 2017 Christian Sailer
# SPDX-FileCopyrightText: 2017 Petros Koutsolampros
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(qtguiTest qtguiTest)

if (NOT FORCE_GLOBAL_COMPILE_WARNINGS)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        # reducing the warning level for catch2
        set(COMPILE_WARNINGS -Wall -Wextra)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(COMPILE_WARNINGS /W4 /EHsc)
    endif()
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)

# Find the QtWidgets library
find_package(Qt6 COMPONENTS Core Widgets Gui OpenGL REQUIRED)

include_directories(${QT})

add_compile_definitions(_DEPTHMAP)

set(qtguiTest_SRCS
    ../ThirdParty/Catch/catch_amalgamated.cpp
    main.cpp
    selfcleaningfile.hpp
    testgllines.cpp
    testgllinesuniform.cpp
    testglrastertexture.cpp
    testviewhelpers.cpp
    testsettings.cpp
    testmetagraphdx.cpp
    ../qtgui/settingsimpl.cpp
    ../qtgui/dminterface/shapemapdm.cpp
    ../qtgui/dminterface/latticemapdm.cpp
    ../qtgui/dminterface/shapegraphdm.cpp
    ../qtgui/dminterface/metagraphdm.cpp
    ../qtgui/views/viewhelpers.cpp
    ../qtgui/views/glview/gllines.hpp
    ../qtgui/views/glview/gllines.cpp
    ../qtgui/views/glview/gllinesuniform.hpp
    ../qtgui/views/glview/gllinesuniform.cpp
    ../qtgui/views/glview/glrastertexture.hpp
    ../qtgui/views/glview/glrastertexture.cpp)


include_directories("../ThirdParty/Catch")

set(LINK_LIBS salalib)

if(APPLE)
        add_definitions(-DGL_SILENCE_DEPRECATION)
endif(APPLE)

add_executable(${qtguiTest} ${qtguiTest_SRCS})

target_compile_options(${qtguiTest} PRIVATE ${COMPILE_WARNINGS})

if ((MSVC) AND (MSVC_VERSION GREATER_EQUAL 1914))
    # new option required from MSVC, but not yet implemented in CMake
    # see: https://gitlab.kitware.com/cmake/cmake/-/issues/18837
    target_compile_options(${qtguiTest} PUBLIC "/Zc:__cplusplus" "-permissive-")
endif()

find_package(OpenGL REQUIRED)

target_link_libraries(${qtguiTest} Qt6::OpenGL salalib OpenGL::GL)

