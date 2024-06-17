// SPDX-FileCopyrightText: 2021 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "coreapplication.h"

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(resource);
    Q_INIT_RESOURCE(settingsdialog);

    CoreApplication app(argc, argv);

    return app.exec();
}
