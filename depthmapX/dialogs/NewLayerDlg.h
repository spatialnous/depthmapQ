// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_NewLayerDlg.h"

class CNewLayerDlg : public QDialog, public Ui::CNewLayerDlg {
    Q_OBJECT
  public:
    CNewLayerDlg(QWidget *parent = 0);
    int m_layer_type;
    QString m_name;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnSelchangeLayerType(int);
    void OnOK();
    void OnCancel();
};
