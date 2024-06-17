// SPDX-FileCopyrightText: 2022 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "treeitem.h"

#include "agl/composite/aglmap.h"

#include "salalib/attributetable.h"

#include <QObject>
#include <QString>

#include <memory>

class MapLayer : public QObject, public TreeItem {
    Q_OBJECT
    Q_PROPERTY(QString name MEMBER m_name NOTIFY nameChanged)
    Q_PROPERTY(bool visible MEMBER m_visible NOTIFY visibilityChanged)

  protected:
    AttributeTable &m_attributes;

  public:
    MapLayer(QString mapName, AttributeTable &attributes)
        : TreeItem(mapName), m_attributes(attributes){};

    bool isVisible() { return m_visible; }
    QString getName() { return m_name; }
    AttributeTable &getAttributes() { return m_attributes; }

    virtual std::unique_ptr<AGLMap> constructGLMap() = 0;

    virtual bool hasGraph() { return false; }

  signals:
    void nameChanged();
    void visibilityChanged();
};
