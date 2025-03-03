// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "settingspage.hpp"

#include <QWidget>
#include <QtWidgets>

#include <map>
#include <memory>

class InterfacePage : public SettingsPage {
    Q_OBJECT

  private:
    QColor m_background;
    QColor m_foreground;
    std::map<QListWidgetItem *, QColor *> colourMap;
    int m_antialiasingSamples = 0;
    bool m_defaultMapWindowIsLegacy = false;
    bool m_highlightOnHover = true;
    void readSettings(Settings &settings) {
        m_foreground =
            QColor(settings.readSetting(SettingTag::foregroundColour, qRgb(128, 255, 128)).toInt());
        m_background =
            QColor(settings.readSetting(SettingTag::backgroundColour, qRgb(0, 0, 0)).toInt());
        m_antialiasingSamples = settings.readSetting(SettingTag::antialiasingSamples, 0).toInt();
        m_defaultMapWindowIsLegacy =
            settings.readSetting(SettingTag::legacyMapWindow, false).toBool();
        m_highlightOnHover = settings.readSetting(SettingTag::highlightOnHover, true).toBool();
    }
  private slots:
    void onInterfaceColourlItemClicked(QListWidgetItem *item);

  public:
    InterfacePage(Settings &settings, QWidget *parent = 0);
    virtual void writeSettings(Settings &settings) override {
        settings.writeSetting(SettingTag::backgroundColour, m_background.rgb());
        settings.writeSetting(SettingTag::foregroundColour, m_foreground.rgb());
        settings.writeSetting(SettingTag::antialiasingSamples, m_antialiasingSamples);
        settings.writeSetting(SettingTag::legacyMapWindow, m_defaultMapWindowIsLegacy);
        settings.writeSetting(SettingTag::highlightOnHover, m_highlightOnHover);
    }
};
