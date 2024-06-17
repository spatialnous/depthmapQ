// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PromptReplace.h"

CPromptReplace::CPromptReplace(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_message = tr("");

    UpdateData(false);
}

void CPromptReplace::OnAdd() {
    UpdateData(true);
    done(1);
}

void CPromptReplace::OnReplace() {
    UpdateData(true);
    done(2);
}

void CPromptReplace::OnCancel() { reject(); }

void CPromptReplace::UpdateData(bool value) {
    if (value)
        m_message = c_message->text();
    else
        c_message->setText(m_message);
}

void CPromptReplace::showEvent(QShowEvent *event) { UpdateData(false); }
