// SPDX-FileCopyrightText: 2021 - 2022 Petros Koutsolampros
// SPDX-License-Identifier: GPL-3.0-or-later
pragma Singleton

import QtQuick

QtObject {
    property color focusColour: "red"
    property color panelColour: "#555555"
    property color panelTextColour: "#FFFFFF"
    property color toolbarButtonColour: "transparent"
    property color toolbarButtonHoverColour: "#777777"
    property color toolbarButtonTextColour: "white"
    property color toolbarButtonTextDisabledColour: "#777777"
    property color activeTabColour: "#888888"
    property color activeTabHoverColour: activeTabColour
    property color inactiveTabColour: "#444444"
    property color inactiveTabHoverColour: toolbarButtonHoverColour
    property color tabCloseButtonColour: "transparent"
    property color tabCloseButtonHoverColour: "#828282"
    property color appNameColour: "#dddddd"
    property int tooltipDelay: Qt.styleHints.mousePressAndHoldInterval
    property int tabButtonHoverRadius: 5
}
