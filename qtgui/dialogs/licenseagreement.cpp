// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "licenseagreement.hpp"

LicenseAgreement::LicenseAgreement(QWidget *parent)
    : QDialog(parent), ui(new Ui::LicenseAgreement) {
    ui->setupUi(this);
}

LicenseAgreement::~LicenseAgreement() { delete ui; }
