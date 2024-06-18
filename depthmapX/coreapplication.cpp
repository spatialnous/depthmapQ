// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "coreapplication.h"

#include "interfaceversion.h"
#include "mainwindowfactory.h"
#include "settingsimpl.h"

int CoreApplication::exec() {
    SettingsImpl settings(new DefaultSettingsFactory);

    if (!settings.readSetting(SettingTag::licenseAccepted, false).toBool()) {
        auto dummy = MainWindowFactory::getLicenseDialog();
        dummy->setModal(true);
        dummy->setWindowTitle(TITLE_BASE);
        dummy->exec();
        if (dummy->result() == QDialog::Rejected) {
            return 0;
        }
        settings.writeSetting(SettingTag::licenseAccepted, true);
    }

    auto args = arguments();
    QString fileToLoad = mFileToLoad;
    if (args.length() == 2) {
        fileToLoad = args[1];
    }

    mMainWindow = MainWindowFactory::getMainWindow(fileToLoad, settings);
    mMainWindow->show();

    QApplication::setWindowIcon(QIcon(":/images/depthmapX.png"));

    return QApplication::exec();
}
