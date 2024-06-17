// SPDX-FileCopyrightText: 2021 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "licenseagreement.h"

LicenseAgreement::LicenseAgreement(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::LicenseAgreement) {
    m_ui->setupUi(this);
}

LicenseAgreement::~LicenseAgreement() { delete m_ui; }
