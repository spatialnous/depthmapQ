// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "documentmanager.h"

#include <QFileOpenEvent>
#include <QGuiApplication>
#include <QtDebug>

class CoreApplication : public QGuiApplication {
  private:
    std::string m_fileToLoad;

  public:
    CoreApplication(int &argc, char **argv) : QGuiApplication(argc, argv) {}

    bool event(QEvent *event) {
        // this event is triggered in macOS, either by calling "Open with..."
        // in Finder, or by dropping a file on the acanthis icon on the dock
        // more info: http://doc.qt.io/qt-5/qfileopenevent.html
        if (event->type() == QEvent::FileOpen) {
            QFileOpenEvent *openEvent = static_cast<QFileOpenEvent *>(event);
            m_fileToLoad = openEvent->file().toStdString();
        }

        return QGuiApplication::event(event);
    }

    int exec();
};
