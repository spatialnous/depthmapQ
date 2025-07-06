// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FewestLineOptionsDlg.hpp"

CFewestLineOptionsDlg::CFewestLineOptionsDlg(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_option = false;
    UpdateData(false);
}

void CFewestLineOptionsDlg::OnOK() {
    UpdateData(true);
    accept();
}

void CFewestLineOptionsDlg::OnCancel() { reject(); }

void CFewestLineOptionsDlg::UpdateData(bool value) {
    if (value) {
        m_option = c_option->isChecked();
    } else {
        c_option->setChecked(m_option);
    }
}

void CFewestLineOptionsDlg::showEvent(QShowEvent *event) { UpdateData(false); }
