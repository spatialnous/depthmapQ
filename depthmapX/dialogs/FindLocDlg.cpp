// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "FindLocDlg.hpp"

#include <QMessageBox>

CFindLocDlg::CFindLocDlg(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_x = 0.0;
    m_y = 0.0;

    UpdateData(false);
}

void CFindLocDlg::OnOK() {
    UpdateData(true);
    QPoint p((int)m_x, (int)m_y);
    if (!m_bounds.contains(p)) {
        QMessageBox::warning(this, tr("Warning"),
                             tr("This point is outside the bounds of your map"), QMessageBox::Ok,
                             QMessageBox::Ok);
        return;
    }
    accept();
}

void CFindLocDlg::OnCancel() { reject(); }

void CFindLocDlg::UpdateData(bool value) {
    if (value) {
        m_x = c_x->text().toDouble();
        m_y = c_y->text().toDouble();
    } else {
        c_x->setText(QString("%1").arg(m_x));
        c_y->setText(QString("%1").arg(m_y));
    }
}

void CFindLocDlg::showEvent(QShowEvent *event) { UpdateData(false); }
