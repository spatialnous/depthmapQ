// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "NewLayerDlg.hpp"

#include <QMessageBox>

CNewLayerDlg::CNewLayerDlg(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_layer_type = 0;
    m_name = tr("Gate Map");

    UpdateData(false);
}

void CNewLayerDlg::OnSelchangeLayerType(int value) {
    int which = value;

    switch (which) {
    case 0:
        c_name->setText(tr("Gate Map"));
        break;
    case 1:
        c_name->setText(tr("Convex Map"));
        break;
    case 2:
        c_name->setText(tr("Axial Map"));
        break;
    case 3:
        c_name->setText(tr("Pesh Map"));
        break;
    }
}

void CNewLayerDlg::OnOK() {
    UpdateData(true);

    if (m_name.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Please enter a name for the new map"),
                             QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    accept();
}

void CNewLayerDlg::OnCancel() { reject(); }

void CNewLayerDlg::UpdateData(bool value) {
    if (value) {
        m_layer_type = c_layer_selector->currentIndex();
        m_name = c_name->text();
    } else {
        c_layer_selector->setCurrentIndex(m_layer_type);
        c_name->setText(m_name);
    }
}

void CNewLayerDlg::showEvent(QShowEvent *event) { UpdateData(false); }
