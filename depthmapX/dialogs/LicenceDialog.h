// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_LicenceDialog.h"

class CLicenceDialog : public QDialog, public Ui::CLicenceDialog {
    Q_OBJECT
  public:
    CLicenceDialog(QWidget *parent = 0);
    QString m_message;
    QString m_agreement;

    QString m_title;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnCancel();
};
