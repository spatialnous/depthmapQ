// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "LicenceDialog.hpp"

const char *g_agreement = "see http://www.gnu.org/licenses/";

CLicenceDialog::CLicenceDialog(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_message = tr("");
    m_agreement = tr("");
}

void CLicenceDialog::OnOK() {
    UpdateData(true);
    accept();
}

void CLicenceDialog::OnCancel() { reject(); }

void CLicenceDialog::UpdateData(bool value) {
    if (value) {
        m_message = c_message->text();
        m_agreement = c_agreement->toPlainText();
    } else {
        c_message->setText(m_message);
        c_agreement->setPlainText(m_agreement);
    }
}

void CLicenceDialog::showEvent(QShowEvent *event) {
    setWindowTitle(m_title);

    m_message = tr("old");
    m_agreement = QString("old") + tr("old") + QString("old") + g_agreement;

    UpdateData(false);
}
