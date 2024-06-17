// SPDX-FileCopyrightText: 2020 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "mainwindow.h"

#include <QMenu>

namespace MainWindowHelpers {
    QMenu *getOrAddRootMenu(MainWindow *mainWindow, QString menuTitle);
    QMenu *getOrAddMenu(QMenu *parent, QString menuTitle);
} // namespace MainWindowHelpers
