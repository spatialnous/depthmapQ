// SPDX-FileCopyrightText: 2021 - 2022 Petros Koutsolampros
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Window

import acanthis 1.0

TreeView {
    id: mapLayersTree
    property var graphViewModel

    ScrollBar.vertical: ScrollBar {}

    model: AQMapViewModel {
        id: memodl
        graphViewModel: mapLayersTree.graphViewModel
        Component.onCompleted: {
            memodl.resetItems()
        }
    }

    delegate: Item {
        id: root

        function getImplicitWidth() {
            switch (model.column) {
            case 0:
                // visible
                // first column, also contains the tree arrows
                return padding + visibilityCheckbox.implicitWidth
            case 1:
                // editable
            default:
                return padding + visibilityCheckbox.implicitWidth
            case nbuttons:
                // label
                return label.implicitWidth + padding
            }
        }
        function getModelIndex() {
            // TreeView inherits from TableView, but employs various
            // hacks to actually display the table as a tree. In order
            // to achieve this, there is an intermediary model that
            // translates the visible rows (collapsed or expanded) into
            // the rows of the tree and vice versa. The model we provide
            // for the TreeView works with the translated indices that
            // are tree like, while the view (and what we get from calling
            // model.row and model.row) has indices that relate to what's
            // visible at the time. The modelIndex function allows us to
            // do the translation as well and get the proper index to query
            // or modify a treeitem from our model.
            let midx = modelIndex(model.row, model.column)
            if (qtversion === "6.4.2") {
                // Because of an API incompatible change between Qt 6.4.0
                // and Qt 6.4.2, the order of row and column was specified
                // in the opposite order
                // https://doc.qt.io/qt-6.5/qml-qtquick-tableview-obsolete.html#modelIndex-method
                midx = modelIndex(model.column, model.row)
            }
            return midx
        }

        implicitWidth: getImplicitWidth()
        implicitHeight: visibilityCheckbox.implicitHeight * 1.5

        readonly property real indent: 10
        readonly property real padding: 5

        required property TreeView treeView
        required property bool isTreeNode
        required property bool expanded
        required property int hasChildren
        required property int depth

        readonly property int nbuttons: 2

        Text {
            id: visibilityCheckbox
            visible: model.column === 0
            text: model.visibility ? "👁" : "H"
            color: Theme.toolbarButtonTextColour
            TapHandler {
                onTapped: {
                    memodl.setItemVisible(getModelIndex(),
!model.visibility)
                    graphViews.redraw();                }
            }
        }
        Text {
            visible: model.column === 1
            text: model.editability ? "E" : "L"
            color: Theme.toolbarButtonTextColour
            TapHandler {
                onTapped: {
                    memodl.setItemEditable(getModelIndex(),
!model.editability)
                    graphViews.redraw();                }
            }
        }
        Text {
            x: (root.depth * root.indent)
            visible: root.hasChildren && model.column === nbuttons
            text: root.expanded ? "▼" : "▶"
            color: Theme.toolbarButtonTextColour
            TapHandler {
                onTapped: {
                    treeView.toggleExpanded(row)
                }
            }
        }
        Text {
            id: label
            // The actual text is properly placed at the end of all icons (except the arrow).
            // Only push out the width of the arrow
            x: (visibilityCheckbox.width + padding) + (root.depth * root.indent)
            visible: model.column === nbuttons
            clip: true
            text: model.name
            color: Theme.toolbarButtonTextColour
            horizontalAlignment: Text.AlignLeft
        }
    }
}
