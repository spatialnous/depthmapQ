// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "LayerChooserDlg.h"

CLayerChooserDlg::CLayerChooserDlg(const std::vector<std::string> &names, QWidget *parent)
    : QDialog(parent), m_names(names) {
    setupUi(this);
    m_text = tr("");
    m_layer = 0;
}

void CLayerChooserDlg::OnOK() {
    UpdateData(true);
    accept();
}

void CLayerChooserDlg::OnCancel() { reject(); }

void CLayerChooserDlg::UpdateData(bool value) {
    if (value) {
        m_text = c_text->text();
        m_layer = c_layer_selector->currentIndex();
    } else {
        c_text->setText(m_text);
        c_layer_selector->setCurrentIndex(m_layer);
    }
}

void CLayerChooserDlg::showEvent(QShowEvent *event) {
    for (size_t i = 0; i < m_names.size(); i++) {
        c_layer_selector->addItem(QString(m_names[i].c_str()));
    }
    c_layer_selector->setCurrentIndex(0);

    UpdateData(false);
}
