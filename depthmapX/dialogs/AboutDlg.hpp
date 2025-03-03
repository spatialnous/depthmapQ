// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ui_AboutDlg.h"

class CAboutDlg : public QDialog, public Ui::CAboutDlg {
    Q_OBJECT
  public:
    CAboutDlg(QWidget *parent = 0);

  private slots:
    void OnOK();
};
