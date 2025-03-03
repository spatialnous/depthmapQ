// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ui_FindLocDlg.h"

class CFindLocDlg : public QDialog, public Ui::CFindLocDlg {
    Q_OBJECT
  public:
    CFindLocDlg(QWidget *parent = 0);
    double m_x;
    double m_y;
    QRegion m_bounds;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnCancel();
};
