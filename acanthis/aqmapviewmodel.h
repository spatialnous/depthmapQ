// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "graphviewmodel.h"

#include <QAbstractListModel>

class AQMapViewModel : public QAbstractItemModel {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(GraphViewModel *graphViewModel MEMBER m_graphViewModel NOTIFY graphViewModelChanged)

    GraphViewModel *m_graphViewModel;
    QSharedPointer<TreeItem> m_rootItem;
    TreeItem *getItem(const QModelIndex &idx) const;

    // Add roles that are not visible at the bottom only.
    // If new column roles are added, then the number
    // underneath must also be incremented
    enum LayerModelRole {
        // Roles visible as columns
        NameRole = Qt::UserRole,
        VisibleRole,
        EditableRole
    };

    short numRolesAsColumns = 3;

    QSharedPointer<TreeItem> addChildItem(QSharedPointer<TreeItem> parent,
                                          QSharedPointer<TreeItem> newChild, int row);
    QSharedPointer<TreeItem> addChildItem(QSharedPointer<TreeItem> parent, QString newChild,
                                          int row);

  public:
    explicit AQMapViewModel(QObject *parent = nullptr);

    void updateAfterGraphViewModelChange();

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    AQMapViewModel::LayerModelRole getRole(int columnIndex) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void resetItems();
    Q_INVOKABLE void setItemVisible(const QModelIndex &idx, bool visibility);
    Q_INVOKABLE void setItemEditable(const QModelIndex &idx, bool editability);

  signals:
    void graphViewModelChanged();
};
