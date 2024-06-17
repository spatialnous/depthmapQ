// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_FewestLineOptionsDlg.h"

class CFewestLineOptionsDlg : public QDialog, public Ui::CFewestLineOptionsDlg {
    Q_OBJECT
  public:
    CFewestLineOptionsDlg(QWidget *parent = 0);
    bool m_option;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnCancel();
};
