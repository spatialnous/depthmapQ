// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "treeitem.h"

#include "salalib/attributetable.h"

#include <QObject>
#include <QString>

class AttributeItem : public QObject, public TreeItem {
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged)
    Q_PROPERTY(bool visible MEMBER m_visible NOTIFY visibilityChanged)

    AttributeColumn &m_attributeColumn;

  public:
    AttributeItem(AttributeColumn &attributeColumn)
        : TreeItem(QString::fromStdString(attributeColumn.getName())),
          m_attributeColumn(attributeColumn) {
        m_visible = false;
    };

  signals:
    void nameChanged();
    void visibilityChanged();
};
