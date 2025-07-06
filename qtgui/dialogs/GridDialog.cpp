// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
// SPDX-FileCopyrightText: 2017 Christian Sailer
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "GridDialog.hpp"

#include "salalib/gridproperties.hpp"

#include <QMessageBox>

CGridDialog::CGridDialog(double maxDimension, QWidget *parent)
    : QDialog(parent), m_maxdimension(maxDimension) {
    setupUi(this);
    m_spacing = 0.01;
}

void CGridDialog::showEvent(QShowEvent *event) {
    GridProperties gp(m_maxdimension);
    m_spacing = gp.getDefault();

    c_spacing_ctrl->setRange(gp.getMin(), gp.getMax());

    UpdateData(false);
}

void CGridDialog::OnDeltaposSpinSpacing(double iDelta) {
    // New slot for this ready userValue(double value) // slot link here though // TV
    // m_spacing = c_spacing_ctrl->value(); // bug or not?

    if (int(iDelta / 1.0) > 1)
        c_spacing_ctrl->setSingleStep(1);
    else if (int(iDelta / 0.1) > 1)
        c_spacing_ctrl->setSingleStep(0.1);
    else if (int(iDelta / 0.01) > 1)
        c_spacing_ctrl->setSingleStep(0.01);
    else
        c_spacing_ctrl->setSingleStep(0.001);
}

void CGridDialog::OnOK() {
    UpdateData(true);
    accept();
}

void CGridDialog::OnCancel() { reject(); }

void CGridDialog::UpdateData(bool value) {
    if (value) {
        m_spacing = c_spacing_ctrl->value();
    } else {
        c_spacing_ctrl->setValue(m_spacing);
    }
}
