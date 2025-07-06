// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "generalpage.hpp"
#include "settings.hpp"

#include <QtWidgets>

GeneralPage::GeneralPage(Settings &settings, QWidget *parent) : SettingsPage(settings, parent) {
    readSettings(settings);
    QGroupBox *configGroup = new QGroupBox(tr("General configuration"));
    QCheckBox *simpleModeCheckBox = new QCheckBox(tr("Simple mode"));
    simpleModeCheckBox->setToolTip(tr("If enabled, only Integration [HH] will be calulcated (or "
                                      "Visual Integration [HH] for VGA)"));
    simpleModeCheckBox->setChecked(m_simpleVersion);
    connect(simpleModeCheckBox, &QCheckBox::stateChanged,
            [=]() { m_simpleVersion = !m_simpleVersion; });

    QVBoxLayout *configLayout = new QVBoxLayout;
    configLayout->addWidget(simpleModeCheckBox);
    configGroup->setLayout(configLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configGroup);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}
