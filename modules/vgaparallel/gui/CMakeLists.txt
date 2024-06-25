# SPDX-FileCopyrightText: 2024 Petros Koutsolampros
#
# SPDX-License-Identifier: GPL-3.0-or-later

set(module vgaparallel)
set(module_SRCS
    vgaparallelmainwindow.h
    vgaparallelmainwindow.cpp)
set(modules_gui "${modules_gui}" ${module} CACHE INTERNAL "modules_gui" FORCE)

find_package(Qt6 COMPONENTS Core Widgets Gui OpenGLWidgets OpenGL REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Qt6Core_INCLUDE_DIRS})
include_directories(${Qt6Widgets_INCLUDE_DIRS})
include_directories(${Qt6Gui_INCLUDE_DIRS})
include_directories(${Qt6OpenGL_INCLUDE_DIRS})
include_directories(${Qt6OpenGLWidgets_INCLUDE_DIRS})

set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "../../../depthmapX/UI")

add_definitions(${Qt6Core_DEFINITIONS})
add_definitions(${Qt6Widgets_DEFINITIONS})
add_definitions(${Qt6Gui_DEFINITIONS})
add_definitions(${Qt6OpenGL_DEFINITIONS})
add_definitions(${Qt6OpenGLWidgets_DEFINITIONS})

add_compile_definitions(VGAPARALLEL_GUI_LIBRARY)

add_library(${module} OBJECT ${module_SRCS}
    ../../../depthmapX/imainwindowmodule.h)
