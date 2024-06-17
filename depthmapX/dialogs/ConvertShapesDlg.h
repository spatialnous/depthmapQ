// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_ConvertShapesDlg.h"

class CConvertShapesDlg : public QDialog, public Ui::CConvertShapesDlg {
    Q_OBJECT
  public:
    CConvertShapesDlg(QWidget *parent = 0);
    double m_radius;
    bool m_selected_only;
    int m_conversion_type;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnCancel();
};
