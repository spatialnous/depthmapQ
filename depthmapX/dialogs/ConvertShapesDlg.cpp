// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ConvertShapesDlg.h"

CConvertShapesDlg::CConvertShapesDlg(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_radius = 10.0;
    m_selected_only = false;
    m_conversion_type = 0;
    UpdateData(false);
}

void CConvertShapesDlg::OnOK() {
    UpdateData(true);
    accept();
}

void CConvertShapesDlg::OnCancel() { reject(); }

void CConvertShapesDlg::UpdateData(bool value) {
    double m_radius = 0.0;
    if (value) {
        m_radius = c_radius->text().toDouble();
        if (c_selected_only->checkState())
            m_selected_only = true;
        else
            m_selected_only = false;

        m_conversion_type = c_conversion_type->currentIndex();
    } else {
        c_radius->setText(QString("%1").arg(m_radius));
        if (m_selected_only)
            c_selected_only->setCheckState(Qt::Checked);
        else
            c_selected_only->setCheckState(Qt::Unchecked);

        c_conversion_type->setCurrentIndex(m_conversion_type);
    }
}

void CConvertShapesDlg::showEvent(QShowEvent *event) { UpdateData(false); }
