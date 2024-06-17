// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "settingspage.h"

#include <QWidget>

#include <iostream>

class GeneralPage : public SettingsPage {
  private:
    bool m_simpleVersion = false;
    void readSettings(Settings &settings) {
        m_simpleVersion = settings.readSetting(SettingTag::simpleVersion, true).toBool();
    }

  public:
    GeneralPage(Settings &settings, QWidget *parent = 0);
    virtual void writeSettings(Settings &settings) override {
        settings.writeSetting(SettingTag::simpleVersion, m_simpleVersion);
    }
};
