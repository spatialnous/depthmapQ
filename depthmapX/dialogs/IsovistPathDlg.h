// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_IsovistPathDlg.h"

class CIsovistPathDlg : public QDialog, public Ui::CIsovistPathDlg {
    Q_OBJECT
  public:
    CIsovistPathDlg(QWidget *parent = 0);
    double fov_angle;
    int fov_selection;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnCancel();
};
