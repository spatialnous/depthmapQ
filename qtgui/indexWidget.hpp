// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <QListWidget>

QT_BEGIN_NAMESPACE

class QEvent;
class QListWidgetItem;

class AttribWindow : public QListWidget {
    Q_OBJECT

  public:
    AttribWindow(QWidget *parent = 0, bool custom = true);
    ~AttribWindow();

    QWidget *main_frm;

  private slots:
    void showContextMenu(const QPoint &point);
};

QT_END_NAMESPACE
