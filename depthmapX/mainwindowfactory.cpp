// SPDX-FileCopyrightText: 2017 Christian Sailer
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindowfactory.hpp"

#include "dialogs/licenseagreement.hpp"
#include "mainwindow.hpp"

#include <memory>

namespace MainWindowFactory {
    std::unique_ptr<MainWindow> getMainWindow(const QString &fileToLoad, Settings &settings) {
        return std::unique_ptr<MainWindow>(new MainWindow(fileToLoad, settings));
    }

    std::unique_ptr<QDialog> getLicenseDialog() {
        return std::unique_ptr<QDialog>(new LicenseAgreement);
    }
} // namespace MainWindowFactory
