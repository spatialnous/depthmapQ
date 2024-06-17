// SPDX-FileCopyrightText: 2020 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "depthmapX/mainwindowhelpers.h"
#include "depthmapX/mainwindow.h"

QMenu *MainWindowHelpers::getOrAddRootMenu(MainWindow *mainWindow, QString menuTitle) {
    QMenuBar *menuBar = mainWindow->menuBar();
    QMenu *menu = nullptr;
    foreach (QAction *action, menuBar->actions()) {
        if (action->menu()) {
            QMenu *childMenu = action->menu();
            if (childMenu != nullptr && childMenu->title() == menuTitle) {
                menu = childMenu;
            }
        }
    }
    if (menu == nullptr) {
        menu = menuBar->addMenu(menuTitle);
    }
    return menu;
}

QMenu *MainWindowHelpers::getOrAddMenu(QMenu *parent, QString menuTitle) {
    QMenu *menu = nullptr;
    foreach (QAction *action, parent->actions()) {
        if (action->menu()) {
            QMenu *childMenu = action->menu();
            if (childMenu != nullptr && childMenu->title() == menuTitle) {
                menu = childMenu;
            }
        }
    }
    if (menu == nullptr) {
        menu = parent->addMenu(menuTitle);
    }
    return menu;
}
