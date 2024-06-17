// SPDX-FileCopyrightText: 2021 - 2023 Petros Koutsolampros
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import acanthis 1.0

RowLayout {
    spacing: toolbar.spacing
    id: titlebarRow

    function getModelView() {
        return graphListNameView
    }

    ButtonGroup {
        id: btnGrp
    }
    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true
        RowLayout {
            anchors.fill: parent

            ListView {
                id: graphListNameView
                model: graphDisplayModel
                spacing: toolbar.spacing
                interactive: false
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumWidth: parent.height * 2 * graphDisplayModel.count
                Layout.maximumWidth: parent.height * 10 * graphDisplayModel.count
                orientation: ListView.Horizontal

                delegate: Button {
                    id: tabButton
                    anchors.verticalCenter: parent == null ///
                                            ? window.verticalCenter ///
                                            : parent.verticalCenter
                    ButtonGroup.group: btnGrp
                    width: (graphListNameView.width - ///
                            (graphListNameView.spacing * ///
                             (graphDisplayModel.count + 1))) ///
                           / graphDisplayModel.count
                    height: parent == null ? 0 : parent.height
                    padding: 0
                    onClicked: {
                        graphListNameView.currentIndex = index
                        graphFileView.currentIndex = index
                    }
                    ToolTip.visible: hovered
                    ToolTip.delay: Theme.tooltipDelay
                    ToolTip.text: graphModelFile.getFilename()
                    property int listIndex: index
                    property var fileView: graphFileView
                    function getFileView() {
                        return graphFileView
                    }
                    background: Rectangle {
                        color: {
                            if (hovered) {
                                graphListNameView.currentIndex === index ///
                                        ? Theme.activeTabHoverColour ///
                                        : Theme.inactiveTabHoverColour
                            } else {
                                graphListNameView.currentIndex === index ///
                                        ? Theme.activeTabColour ///
                                        : Theme.inactiveTabColour
                            }
                        }
                        radius: {
                            hovered || graphListNameView.currentIndex
                                    === index ? Theme.tabButtonHoverRadius : 0
                        }
                    }
                    contentItem: RowLayout {
                        spacing: 0
                        Text {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            text: graphModelFile.getFilename()
                            elide: Text.ElideLeft
                            horizontalAlignment: Qt.AlignHCenter
                            verticalAlignment: Qt.AlignVCenter
                            color: Theme.toolbarButtonTextColour

                            // in the case of a long (and thus truncated)
                            // path, this pushes the prefixed ellipsis
                            // further to the right
                            leftPadding: 5
                        }
                        Button {
                            contentItem: Text {
                                text: "✕"
                                horizontalAlignment: Text.AlignHCenter
                                color: Theme.toolbarButtonTextColour
                            }
                            Layout.alignment: Qt.AlignCenter
                            background: Rectangle {
                                width: 21
                                height: 21
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                radius: parent.width * 0.5
                                color: parent.hovered ///
                                       ? Theme.tabCloseButtonHoverColour ///
                                       : Theme.tabCloseButtonColour
                            }
                            onClicked: {
                                graphDisplayModel.remove(index)
                                DocumentManager.removeDocument(listIndex)
                            }
                        }
                    }
                }
            }
            Rectangle {
                // provides an area next to the tabs that allows for
                // dragging the whole window (since it has no titlebar)
                // but also acts as a titlebar when no items are loaded
                Layout.fillHeight: true
                Layout.fillWidth: true

                Layout.minimumWidth: parent.height

                color: Theme.panelColour
                Text {
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: graphDisplayModel.count === 0 ? window.appName : ""
                    color: Theme.appNameColour
                    font.bold: true
                }

                TapHandler {
                    onTapped: if (tapCount === 2)
                                  toggleMaximized()
                    gesturePolicy: TapHandler.DragThreshold
                }
                DragHandler {
                    grabPermissions: TapHandler.CanTakeOverFromAnything
                    target: null
                    onActiveChanged: if (active) {
                                         window.startSystemMove()
                                     }
                }
            }
        }
    }
    Button {
        contentItem: Text {
            text: "✕"
            horizontalAlignment: Text.AlignHCenter
            color: Theme.toolbarButtonTextColour
        }
        Layout.alignment: Qt.AlignCenter
        background: Rectangle {
            width: 21
            height: 21
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            radius: parent.width * 0.5
            color: parent.hovered ///
                   ? Theme.toolbarButtonHoverColour ///
                   : Theme.toolbarButtonColour
        }
        onClicked: {
            Qt.quit()
        }
    }
}
