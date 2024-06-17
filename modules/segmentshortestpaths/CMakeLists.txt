# SPDX-FileCopyrightText: 2020 Petros Koutsolampros
#
# SPDX-License-Identifier: GPL-3.0-or-later

if(MODULES_CORE)
add_subdirectory(core)
endif()

if(MODULES_GUI)
  add_subdirectory(gui)
endif()

if(MODULES_CLI)
  add_subdirectory(cli)
endif()

if(MODULES_CORE_TEST)
    add_subdirectory(coreTest)
endif()

if(MODULES_CLI_TEST)
    add_subdirectory(cliTest)
endif()
