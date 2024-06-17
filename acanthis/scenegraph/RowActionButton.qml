// SPDX-FileCopyrightText: 2021 - 2022 Petros Koutsolampros
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "." as Ui

Button {
    id: rowButton
    text: "+"
    flat: true
    focusPolicy: Qt.NoFocus
    hoverEnabled: true

    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.maximumWidth: implicitHeight

    ToolTip.visible: hovered

    contentItem: Text {
        text: rowButton.text
        horizontalAlignment: Text.AlignHCenter
        color: rowButton.enabled ? Theme.toolbarButtonTextColour : Theme.toolbarButtonTextDisabledColour
    }

    background: Rectangle {
        Layout.fillHeight: true
        implicitWidth: parent.height
        radius: Theme.tabButtonHoverRadius
        color: parent.hovered ? Theme.toolbarButtonHoverColour : Theme.toolbarButtonColour
    }
}
