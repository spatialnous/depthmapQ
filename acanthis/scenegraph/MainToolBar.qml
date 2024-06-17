// SPDX-FileCopyrightText: 2021 - 2022 Petros Koutsolampros
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

import acanthis 1.0

RowLayout {
    spacing: toolbar.spacing
    id: toolbarRow

    FileDialog {
        id: openDialog
        onAccepted: window.openDocument(openDialog.selectedFile)
    }

    ToolButton {
        id: newButton
        onClicked: window.newDocument()
        Layout.fillHeight: true
        contentItem: Text {
            text: "🗎"
            horizontalAlignment: Text.AlignHCenter
            color: Theme.toolbarButtonTextColour
        }
        background: Rectangle {
            Layout.fillHeight: true
            implicitWidth: parent.height
            radius: Theme.tabButtonHoverRadius
            color: parent.hovered ? Theme.toolbarButtonHoverColour : Theme.toolbarButtonColour
        }
    }

    ToolButton {
        id: openButton
        onClicked: openDialog.open()
        Layout.fillHeight: true
        contentItem: Text {
            text: "📂"
            horizontalAlignment: Text.AlignHCenter
            color: Theme.toolbarButtonTextColour
        }
        background: Rectangle {
            Layout.fillHeight: true
            implicitWidth: parent.height
            radius: Theme.tabButtonHoverRadius
            color: parent.hovered ? Theme.toolbarButtonHoverColour : Theme.toolbarButtonColour
        }
    }

    ToolButton {
        id: saveButton
        onClicked: {
            console.log("UNIMPLEMENTED: Save file " + graphDisplayModel.get(
                            graphListNameView.currentIndex).graphModelFile.getFilename(
                            ))
        }
        Layout.fillHeight: true
        contentItem: Text {
            text: "💾"
            horizontalAlignment: Text.AlignHCenter
            color: Theme.toolbarButtonTextColour
        }
        background: Rectangle {
            Layout.fillHeight: true
            implicitWidth: parent.height
            radius: Theme.tabButtonHoverRadius
            color: parent.hovered ? Theme.toolbarButtonHoverColour : Theme.toolbarButtonColour
        }
    }

    ToolButton {
        id: rightButton
        contentItem: Text {
            text: "☰"
            horizontalAlignment: Text.AlignHCenter
            color: Theme.toolbarButtonTextColour
        }
        onClicked: drawer.open()

        Layout.fillHeight: true
        background: Rectangle {
            Layout.fillHeight: true
            implicitWidth: parent.height
            radius: Theme.tabButtonHoverRadius
            color: parent.hovered ? Theme.toolbarButtonHoverColour : Theme.toolbarButtonColour
        }
    }
    ToolButton {
        id: splitHorizontal
        contentItem: Text {
            text: "horizontal"
            horizontalAlignment: Text.AlignHCenter
            color: Theme.toolbarButtonTextColour
        }
        onClicked: {
            graphFileView.currentItem.splitActiveView(Qt.Horizontal);
       }

        Layout.fillHeight: true
        background: Rectangle {
            Layout.fillHeight: true
            implicitWidth: parent.height
            radius: Theme.tabButtonHoverRadius
            color: parent.hovered ? Theme.toolbarButtonHoverColour : Theme.toolbarButtonColour
        }
    }
    ToolButton {
        id: splitVertical
        contentItem: Text {
            text: "vertical"
            horizontalAlignment: Text.AlignHCenter
            color: Theme.toolbarButtonTextColour
        }
        onClicked: {
            graphFileView.currentItem.splitActiveView(Qt.Vertical);
       }

        Layout.fillHeight: true
        background: Rectangle {
            Layout.fillHeight: true
            implicitWidth: parent.height
            radius: Theme.tabButtonHoverRadius
            color: parent.hovered ? Theme.toolbarButtonHoverColour : Theme.toolbarButtonColour
        }
    }
}
