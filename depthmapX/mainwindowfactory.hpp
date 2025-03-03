// SPDX-FileCopyrightText: 2017 Christian Sailer
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "mainwindow.h"

#include <QDialog>

#include <memory>

class Settings;

namespace MainWindowFactory {
    std::unique_ptr<MainWindow> getMainWindow(const QString &fileToLoad, Settings &settings);
    std::unique_ptr<QDialog> getLicenseDialog();
} // namespace MainWindowFactory
