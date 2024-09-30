# SPDX-FileCopyrightText: 2020 Petros Koutsolampros
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(moduleTest moduleTest)

set(moduleTest_SRCS
    ../ThirdParty/Catch/catch_amalgamated.cpp
    main.cpp)

if (NOT FORCE_GLOBAL_COMPILE_WARNINGS)
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
        "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # reducing the warning level for catch2
        set(COMPILE_WARNINGS -Wall -Wextra)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(COMPILE_WARNINGS /W4 /EHsc)
    endif()
endif()

include_directories("../ThirdParty/Catch" "../ThirdParty/FakeIt")

set(modules_coreTest "" CACHE INTERNAL "modules_coreTest" FORCE)
set(MODULES_GUI FALSE)
set(MODULES_CLI FALSE)
set(MODULES_CLI_TEST FALSE)
set(MODULES_CORE FALSE)
set(MODULES_CORE_TEST TRUE)
add_subdirectory(../modules modules)

add_executable(${moduleTest} ${moduleTest_SRCS})

target_compile_options(${moduleTest} PRIVATE ${COMPILE_WARNINGS})

target_link_libraries(${moduleTest} salalib ${modules_coreTest} ${modules_core})
