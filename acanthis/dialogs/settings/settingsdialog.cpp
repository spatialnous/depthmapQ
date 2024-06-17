// SPDX-FileCopyrightText: 2017 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "settingsdialog.h"

#include "generalpage.h"
#include "interfacepage.h"

#include <QtWidgets>

SettingsDialog::SettingsDialog(Settings &settings) : m_settings(settings) {
    contentsWidget = new QListWidget;
    contentsWidget->setIconSize(QSize(96, 84));
    contentsWidget->setMovement(QListView::Static);
    contentsWidget->setMaximumWidth(150);
    contentsWidget->setSpacing(5);

    pagesWidget = new QStackedWidget;
    settingsPages.push_back(std::unique_ptr<SettingsPage>(new GeneralPage(m_settings)));
    settingsPages.push_back(std::unique_ptr<SettingsPage>(new InterfacePage(m_settings)));

    std::vector<std::unique_ptr<SettingsPage>>::iterator iter = settingsPages.begin(),
                                                         end = settingsPages.end();
    for (; iter != end; ++iter) {
        pagesWidget->addWidget((*iter).get());
    }

    QPushButton *saveButton = new QPushButton(tr("Save"));
    connect(saveButton, &QAbstractButton::clicked, this, &SettingsDialog::saveChangesAndClose);

    QPushButton *cancelButton = new QPushButton(tr("Cancel"));
    connect(cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);

    createIcons();
    contentsWidget->setCurrentRow(0);

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->addWidget(cancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Settings"));
}

void SettingsDialog::saveChanges() {
    std::vector<std::unique_ptr<SettingsPage>>::iterator iter = settingsPages.begin(),
                                                         end = settingsPages.end();
    for (; iter != end; ++iter) {
        (*iter)->writeSettings(m_settings);
    }
}

void SettingsDialog::saveChangesAndClose() {
    saveChanges();
    QDialog::accept();
}

void SettingsDialog::createIcons() {
    QListWidgetItem *generalButton = new QListWidgetItem(contentsWidget);
    generalButton->setIcon(QIcon(":/images/general.png"));
    generalButton->setText(tr("General"));
    generalButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QListWidgetItem *interfaceButton = new QListWidgetItem(contentsWidget);
    interfaceButton->setIcon(QIcon(":/images/interface.png"));
    interfaceButton->setText(tr("Interface"));
    interfaceButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    connect(contentsWidget, &QListWidget::currentItemChanged, this, &SettingsDialog::changePage);
}

void SettingsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous) {
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}
