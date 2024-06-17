// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "RenameObjectDlg.h"

CRenameObjectDlg::CRenameObjectDlg(const QString &object_type, const QString &existing_name,
                                   QWidget *parent)
    : QDialog(parent) {
    setupUi(this);
    m_prompt = tr("");
    m_object_name = tr("");
    m_object_name = existing_name;

    m_object_type = object_type; // e.g., Column, Layer, etc
}

void CRenameObjectDlg::OnOK() {
    UpdateData(true);
    accept();
}

void CRenameObjectDlg::OnCancel() { reject(); }

void CRenameObjectDlg::UpdateData(bool value) {
    if (value) {
        m_object_name = c_object_name->text();
        m_prompt = c_prompt->text();
    } else {
        c_object_name->setText(m_object_name);
        c_prompt->setText(m_prompt);
    }
}

void CRenameObjectDlg::showEvent(QShowEvent *event) {
    QString lower_object_type = m_object_type;
    lower_object_type = lower_object_type.toLower();

    if (m_object_name.isEmpty()) {
        QString title = QString("New ") + m_object_type;
        setWindowTitle(title);
        m_prompt = QString("New ") + lower_object_type + QString(" name:");
        m_object_name = QString("<New ") + m_object_type + QString(">");
    } else {
        QString title = QString("Rename ") + m_object_type;
        setWindowTitle(title);
        m_prompt = QString("Rename ") + lower_object_type + QString(" to:");
    }
    UpdateData(false);
}
