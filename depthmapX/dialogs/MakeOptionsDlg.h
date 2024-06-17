// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_MakeOptionsDlg.h"

class CMakeOptionsDlg : public QDialog, public Ui::CMakeOptionsDlg {
    Q_OBJECT
  public:
    CMakeOptionsDlg(QWidget *parent = 0);
    bool m_boundarygraph;
    double m_maxdist;
    bool m_restrict_visibility;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnRestrict(bool);
    void OnOK();
    void OnCancel();
};
