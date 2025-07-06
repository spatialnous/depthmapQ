// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ui_RenameObjectDlg.h"

class CRenameObjectDlg : public QDialog, public Ui::CRenameObjectDlg {
    Q_OBJECT
  public:
    CRenameObjectDlg(const QString &object_type, const QString &existing_name = QString(),
                     QWidget *parent = 0);
    QString m_object_name;
    QString m_object_type;
    QString m_prompt;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnCancel();
};
