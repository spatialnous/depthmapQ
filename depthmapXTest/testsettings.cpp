// SPDX-FileCopyrightText: 2017 Christian Sailer
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "selfcleaningfile.h"

#include "depthmapX/settingsimpl.h"

#include "catch_amalgamated.hpp"

class TestSettingsFactory : public QSettingsFactory {
  public:
    TestSettingsFactory(const QString &filename) : mFilename(filename) {}
    virtual std::unique_ptr<QSettings> getSettings() const {
        return std::unique_ptr<QSettings>(new QSettings(mFilename, QSettings::IniFormat));
    }

  private:
    QString mFilename;
};

TEST_CASE("Test simple settings") {
    SelfCleaningFile scf("./test.ini");
    SettingsImpl settings(new TestSettingsFactory(scf.Filename().c_str()));

    REQUIRE(settings.readSetting("test1", "bar").toString().toStdString() == "bar");
    settings.writeSetting("test1", "foo");
    REQUIRE(settings.readSetting("test1", "bar").toString().toStdString() == "foo");
}

TEST_CASE("Test settings transaction") {
    SelfCleaningFile scf("./test.ini");
    SettingsImpl settings(new TestSettingsFactory(scf.Filename().c_str()));

    REQUIRE(settings.readSetting("test1", "bar").toString() == "bar");
    {
        auto transaction = settings.getTransaction();
        transaction->writeSetting("test1", "foo");
    }
    REQUIRE(settings.readSetting("test1", "bar").toString() == "foo");
}
