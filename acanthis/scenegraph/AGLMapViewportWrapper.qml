// SPDX-FileCopyrightText: 2023 - 2023 Petros Koutsolampros
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls

import acanthis 1.0

Page {
    padding: 5
    property string viewID: "-1"
    property bool active: false
    property var graphViewModel
    background: Rectangle {
        //color: settings.glViewBackgroundColour
        border {
            color: active ? "red" : Theme.panelColour
            width: 5
        }
    }
    function update() {
        contentItem.children[0].update()
    }

    AGLMapViewport {
        anchors.fill: parent
        visible: true

        graphViewModel: parent.parent.graphViewModel
        foregroundColour: settings.glViewForegroundColour
        backgroundColour: settings.glViewBackgroundColour
        antialiasingSamples: settings.glViewAntialiasingSamples
        highlightOnHover: settings.glViewHighlightOnHover

        // it is necessary to "flip" the FBO here because the default assumes
        // that y is already flipped. Instead this will be handled internally
        // to be in line with depthmapX
        mirrorVertically: true

        focus: true

        onMousePressed: {
            graphViews.makeActive(parent.parent.viewID)
        }
    }
}
