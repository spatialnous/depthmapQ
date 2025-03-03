// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ui_PromptReplace.h"

class CPromptReplace : public QDialog, public Ui::CPromptReplace {
    Q_OBJECT
  public:
    CPromptReplace(QWidget *parent = 0);
    QString m_message;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnAdd();
    void OnReplace();
    void OnCancel();
};
