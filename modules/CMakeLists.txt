# SPDX-FileCopyrightText: 2020 Petros Koutsolampros
#
# SPDX-License-Identifier: GPL-3.0-or-later

file(GLOB children "*")

# This is a dummy module to allow triggering UIC to build the UI files
add_subdirectory(uictrigger)

foreach(child ${children})
    if(IS_DIRECTORY ${child} AND EXISTS ${child}/CMakeLists.txt AND (NOT ${child} MATCHES "uictrigger$"))
        add_subdirectory(${child})
    endif()
endforeach()
