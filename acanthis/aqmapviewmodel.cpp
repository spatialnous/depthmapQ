// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "aqmapviewmodel.h"

#include "attributeitem.h"

#include <QString>

AQMapViewModel::AQMapViewModel(QObject *parent)
    : QAbstractItemModel(parent), m_rootItem(new TreeItem("Root")) {}

QModelIndex AQMapViewModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    TreeItem *parentItem = getItem(parent);
    auto childPtr = parentItem->getChild(row);
    if (childPtr) {
        return createIndex(row, column, childPtr.get());
    } else {
        return QModelIndex();
    }
}

QModelIndex AQMapViewModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }
    TreeItem *childItem = getItem(index);
    auto parentPtr = childItem->getParent();
    if (!parentPtr || parentPtr == m_rootItem) {
        return QModelIndex();
    }
    return createIndex(parentPtr.get()->getRow(), 0, parentPtr.get());
}

int AQMapViewModel::rowCount(const QModelIndex &parent) const {
    TreeItem *parentItem = getItem(parent);
    return parentItem->nChildren();
}

int AQMapViewModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return numRolesAsColumns;
}

AQMapViewModel::LayerModelRole AQMapViewModel::getRole(int columnIndex) const {
    switch (columnIndex) {
    case 0:
        return VisibleRole;
    case 1:
        return EditableRole;
    case 2:
    default:
        return NameRole;
        // further roles are not shown as columns
    }
}

QVariant AQMapViewModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    TreeItem *item = getItem(index);

    switch (role) {
    case VisibleRole:
        return item->isVisible();
    case EditableRole:
        return item->isEditable();
    case NameRole:
        return item->getName();
    default:
        break;
    }
    return QVariant();
}

QHash<int, QByteArray> AQMapViewModel::roleNames() const {
    QHash<int, QByteArray> names = QAbstractItemModel::roleNames();
    names.insert(QHash<int, QByteArray>{
        {VisibleRole, "visibility"}, {EditableRole, "editability"}, {NameRole, "name"}});
    return names;
}

QSharedPointer<TreeItem> AQMapViewModel::addChildItem(QSharedPointer<TreeItem> parent,
                                                      QSharedPointer<TreeItem> newChild, int row) {
    auto mapItem = parent->addChildItem(newChild, row);
    mapItem->setParentItem(parent);
    return mapItem;
}

QSharedPointer<TreeItem> AQMapViewModel::addChildItem(QSharedPointer<TreeItem> parent,
                                                      QString newChild, int row) {
    return addChildItem(parent, QSharedPointer<TreeItem>(new TreeItem(newChild)), row);
}

void AQMapViewModel::resetItems() {
    if (!m_graphViewModel)
        return;
    beginResetModel();
    int rowL1 = 0;
    for (QSharedPointer<MapLayer> mapLayer : m_graphViewModel->getMapLayers()) {
        QSharedPointer<TreeItem> mapItem = addChildItem(m_rootItem, mapLayer, rowL1);
        mapItem->setVisible(true);
        int rowL2 = 0;
        if (mapLayer->hasGraph()) {
            auto graphItem = addChildItem(mapItem, "Graph", rowL2);
            rowL2++;
        }
        auto allAttrItem = addChildItem(mapItem, "Attributes", rowL2);
        for (int col = 0; col < mapLayer->getAttributes().getNumColumns(); ++col) {
            auto attrItem = addChildItem(allAttrItem,
                                         QSharedPointer<AttributeItem>(new AttributeItem(
                                             mapLayer->getAttributes().getColumn(col))),
                                         col);
        }
        rowL1++;
    }
    endResetModel();
}

void AQMapViewModel::setItemVisible(const QModelIndex &idx, bool visibility) {
    getItem(idx)->setVisible(visibility);
    emit dataChanged(idx, idx, QVector<int>() << VisibleRole);
}

void AQMapViewModel::setItemEditable(const QModelIndex &idx, bool visibility) {
    getItem(idx)->setEditable(visibility);
    emit dataChanged(idx, idx, QVector<int>() << EditableRole);
}

TreeItem *AQMapViewModel::getItem(const QModelIndex &idx) const {
    if (idx.isValid()) {
        TreeItem *item = static_cast<TreeItem *>(idx.internalPointer());
        if (item) {
            return item;
        }
    }
    return m_rootItem.get();
}
