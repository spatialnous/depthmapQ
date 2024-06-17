// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QtCore/QDebug>
#include <QtCore/QEvent>

#include <QtGui/QFocusEvent>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLayout>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>

#include "mainwindow.h"

QT_BEGIN_NAMESPACE

AttribWindow::AttribWindow(QWidget *parent, bool custom) : QListWidget(parent) {
    custom = false;

    main_frm = parent;
    installEventFilter(this);
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this,
            SLOT(showContextMenu(const QPoint &)));

    connect(this, SIGNAL(itemSelectionChanged()), parent, SLOT(OnSelchangingList()));
}

AttribWindow::~AttribWindow() {
    // nothing todo
}

void AttribWindow::showContextMenu(const QPoint &point) {
    QListWidgetItem *item = itemAt(point);
    if (!item)
        return;

    QPoint ptt(mapToGlobal(point));

    ((MainWindow *)main_frm)->showContextMenu(ptt);
}

QT_END_NAMESPACE
