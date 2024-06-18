// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ui_OptionsDlg.h"

class COptionsDlg : public QDialog, public Ui::COptionsDlg {
    Q_OBJECT
  public:
    COptionsDlg(QWidget *parent = 0);
    bool m_global;
    bool m_local;
    QString m_radius;
    bool m_gates_only;
    int m_output_type;
    QString m_radius2;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

    std::vector<std::string> m_layer_names;

  private slots:
    void OnOutputType(bool);
    void OnUpdateRadius(QString);
    void OnUpdateRadius2(QString);
    void OnOK();
    void OnCancel();
};
