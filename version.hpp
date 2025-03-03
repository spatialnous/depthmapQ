// SPDX-FileCopyrightText: 2017 Christian Sailer
// SPDX-FileCopyrightText: 2021 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

// use these to define the depthmap versions
#define DEPTHMAPX_MAJOR_VERSION 0
#define DEPTHMAPX_MINOR_VERSION 9
#define DEPTHMAPX_REVISION_VERSION 0

#define DEPTHMAP_MODULE_VERSION 10.04

// leave these alone - C Preprocessor magic to get stuff to the right format
#define STRINGIFY(v) #v
#define TITLE_BASE_FORMAT(interface, version, minor, revision)                                     \
    interface " " STRINGIFY(version) "." STRINGIFY(minor) "." STRINGIFY(revision)
