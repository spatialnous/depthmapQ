// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "AttributeChooserDlg.h"

CAttributeChooserDlg::CAttributeChooserDlg(AttributeTable &table, QWidget *parent)
    : QDialog(parent) {
    setupUi(this);
    c_attribute_chooser->setCurrentIndex(-1); // m_attribute = -1;
    c_text->setText(QString(tr("")));
    m_table = &table;
}

void CAttributeChooserDlg::OnOK() {
    m_attribute = c_attribute_chooser->currentIndex();
    m_text = c_text->text();
    m_attribute--;
    accept();
}

void CAttributeChooserDlg::OnCancel() { reject(); }

void CAttributeChooserDlg::UpdateData(bool value) {}

void CAttributeChooserDlg::showEvent(QShowEvent *event) {
    c_attribute_chooser->addItem(QString(tr("Ref Number")));
    for (int i = 0; i < m_table->getNumColumns(); i++) {
        c_attribute_chooser->addItem(QString(m_table->getColumnName(i).c_str()));
    }
    c_attribute_chooser->setCurrentIndex(0);
}
