// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "settingspage.hpp"

#include "settings.hpp"

#include <QDialog>

#include <memory>
#include <vector>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class SettingsDialog : public QDialog {
    Q_OBJECT

  public:
    SettingsDialog(Settings &settings);

  public slots:
    void changePage(QListWidgetItem *current, QListWidgetItem *previous);

  private:
    void createIcons();
    std::vector<std::unique_ptr<SettingsPage>> settingsPages;
    QListWidget *contentsWidget;
    QStackedWidget *pagesWidget;
    Settings &m_settings;
    void saveChanges();
    void saveChangesAndClose();
};
