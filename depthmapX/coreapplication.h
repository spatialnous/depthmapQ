// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "mainwindow.h"

#include <QApplication>
#include <QFileOpenEvent>
#include <QtDebug>

class CoreApplication : public QApplication {
  private:
    QString mFileToLoad;
    std::unique_ptr<MainWindow> mMainWindow;

  public:
    CoreApplication(int &argc, char **argv) : QApplication(argc, argv) {}

    bool event(QEvent *event) {
        // this event is triggered in macOS, either by calling "Open with..."
        // in Finder, or by dropping a file on the depthmapX icon on the dock
        // more info: http://doc.qt.io/qt-5/qfileopenevent.html
        if (event->type() == QEvent::FileOpen) {
            QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
            mFileToLoad = openEvent->file();
            mMainWindow->loadFile(openEvent->file());
        }

        return QApplication::event(event);
    }

    int exec();
};
