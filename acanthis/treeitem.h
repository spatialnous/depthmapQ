// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "QVariant"

#include <memory>
#include <vector>

class TreeItem {
    int m_row;
    QWeakPointer<TreeItem> m_parentItem;
    QList<QSharedPointer<TreeItem>> m_children;

  protected:
    QString m_name;
    bool m_visible = true;
    bool m_editable = false;

  public:
    TreeItem(QString name) : m_name(name) {}
    const QSharedPointer<TreeItem> addChildItem(QSharedPointer<TreeItem> treeItem, int row) {
        m_children.push_back(treeItem);
        m_children.back()->setRow(row);
        return m_children.back();
    }

    const QSharedPointer<TreeItem> getParent() const { return m_parentItem; };
    int getRow() const { return m_row; }
    qsizetype nChildren() const { return m_children.size(); }
    const QString getName() const { return m_name; }
    bool isVisible() const { return m_visible; }
    bool isEditable() const { return m_editable; }
    const QSharedPointer<TreeItem> getChild(size_t idx) {
        return m_children.at(static_cast<qsizetype>(idx));
    }

    void setParentItem(QWeakPointer<TreeItem> parent) { m_parentItem = parent; }
    void setRow(int row) { m_row = row; }
    void setVisible(bool visible) { m_visible = visible; }
    void setEditable(bool editable) { m_editable = editable; }
};
