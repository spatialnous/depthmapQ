// SPDX-FileCopyrightText: 2017 Christian Sailer
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "settingsimpl.hpp"

SettingsImpl::SettingsImpl(QSettingsFactory *factory) : mSettingsFactory(factory) {}

const QVariant SettingsImpl::readSetting(const QString &tag, const QVariant &defaultValue) const {
    auto settings = mSettingsFactory->getSettings();
    return settings->value(tag, defaultValue);
}

void SettingsImpl::writeSetting(const QString &tag, const QVariant &value) {
    auto settings = mSettingsFactory->getSettings();
    settings->setValue(tag, value);
}

class SettingsTransactionImpl : public SettingsTransaction {
  public:
    SettingsTransactionImpl(std::unique_ptr<QSettings> &&settings)
        : mSettings(std::move(settings)) {}
    virtual const QVariant readSetting(const QString &tag, const QVariant &defaultValue) const {
        return mSettings->value(tag, defaultValue);
    }
    virtual void writeSetting(const QString &tag, const QVariant &value) {
        mSettings->setValue(tag, value);
    }

  private:
    std::unique_ptr<QSettings> mSettings;
};

std::unique_ptr<SettingsTransaction> SettingsImpl::getTransaction() {
    return std::unique_ptr<SettingsTransaction>(
        new SettingsTransactionImpl(mSettingsFactory->getSettings()));
}
