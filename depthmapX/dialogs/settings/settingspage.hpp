// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "depthmapX/settings.hpp"

#include <QWidget>

class SettingsPage : public QWidget {
  public:
    SettingsPage(Settings &settings, QWidget *parent = 0) : QWidget(parent) {}
    virtual void writeSettings(Settings &settings) = 0;
};
