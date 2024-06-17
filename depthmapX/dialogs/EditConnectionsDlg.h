// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_EditConnectionsDlg.h"

class CEditConnectionsDlg : public QDialog, public Ui::CEditConnectionsDlg {
    Q_OBJECT
  public:
    CEditConnectionsDlg(QWidget *parent = 0);
    bool m_join_type;
    bool m_sel_to_pin;
    bool m_pin_to_sel;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnCancel();
};
