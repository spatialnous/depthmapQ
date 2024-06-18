// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
// SPDX-FileCopyrightText: 2017 Christian Sailer
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ui_GridDialog.h"

class CGridDialog : public QDialog, public Ui::CGridDialog {
    Q_OBJECT
  public:
    CGridDialog(double maxDimension, QWidget *parent = 0);
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);
    double getSpacing() const { return m_spacing; }

  private:
    double m_spacing;
    double m_maxdimension;

  private slots:
    void OnDeltaposSpinSpacing(double);
    void OnOK();
    void OnCancel();
};
