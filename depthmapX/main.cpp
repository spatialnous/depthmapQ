// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
// SPDX-FileCopyrightText: 2017 Christian Sailer
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "coreapplication.h"

#include <QDateTime>
#include <QDir>
#include <QPixmap>

#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char *argv[]) {
    Q_INIT_RESOURCE(resource);
    Q_INIT_RESOURCE(settingsdialog);

    CoreApplication app(argc, argv);

    return app.exec();
}
