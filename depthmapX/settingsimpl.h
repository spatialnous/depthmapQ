// SPDX-FileCopyrightText: 2017 Christian Sailer
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "settings.h"

#include <QSettings>
#include <QStandardPaths>

#include <memory>

class QSettingsFactory {
  public:
    virtual std::unique_ptr<QSettings> getSettings() const = 0;
    virtual ~QSettingsFactory() {}
};

class DefaultSettingsFactory : public QSettingsFactory {
  public:
    DefaultSettingsFactory() {
        m_settingsFile =
            QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation).first() +
            "/depthmapXsettings.ini";
    }

    // QSettingsFactory interface
  public:
    virtual std::unique_ptr<QSettings> getSettings() const {
        return std::unique_ptr<QSettings>(new QSettings(m_settingsFile, QSettings::IniFormat));
    }

  private:
    QString m_settingsFile;
};

class SettingsImpl : public Settings {
  public:
    SettingsImpl(QSettingsFactory *settingsFactory);

    // SettingsTransaction interface
  public:
    virtual const QVariant readSetting(const QString &tag, const QVariant &defaultValue) const;
    virtual void writeSetting(const QString &tag, const QVariant &value);

    // Settings interface
    virtual std::unique_ptr<SettingsTransaction> getTransaction();

  private:
    std::unique_ptr<QSettingsFactory> mSettingsFactory;
};
