// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "treeWindow.h"

#include <QtCore/QDebug>
#include <QtCore/QEvent>

#include <QtGui/QFocusEvent>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTreeWidgetItem>

QT_BEGIN_NAMESPACE

IndexWidget::IndexWidget(QWidget *parent) : QTreeWidget(parent) {
    setColumnCount(2);
    setHeaderLabels(columnNames);
    header()->setSectionResizeMode(Column::MAP, QHeaderView::Stretch);
    header()->setSectionResizeMode(Column::EDITABLE, QHeaderView::ResizeToContents);
    header()->resizeSection(Column::EDITABLE, 10);
    header()->setStretchLastSection(false);

    installEventFilter(this);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(itemChanged(QTreeWidgetItem *, int)), parent,
            SLOT(OnSelchangingTree(QTreeWidgetItem *, int)));
}

void IndexWidget::removeAllItem(QTreeWidgetItem *start) {
    int index;
    QTreeWidgetItem *currentItem = start;
    if (currentItem) {
        QTreeWidgetItem *parent = currentItem->parent();
        if (parent) {
            index = parent->indexOfChild(currentItem);
            delete parent->takeChild(index);
        } else {
            index = indexOfTopLevelItem(currentItem);
            delete takeTopLevelItem(index);
        }
    }
}

QTreeWidgetItem *IndexWidget::addNewItem(const QString &title, QTreeWidgetItem *parent) {
    QTreeWidgetItem *newItem = 0;

    QStringList columnStrings(title);
    if (parent != NULL) {
        newItem = new QTreeWidgetItem(parent, columnStrings);
    } else {
        newItem = new QTreeWidgetItem(this, columnStrings);
    }

    setCurrentItem(newItem);
    newItem->setFlags(newItem->flags() & ~(Qt::ItemIsEditable | Qt::ItemIsSelectable));
    return newItem;
}

QT_END_NAMESPACE
