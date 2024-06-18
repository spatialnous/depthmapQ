// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ui_AxialAnalysisOptionsDlg.h"

#include "salalib/mgraph.h"

class CAxialAnalysisOptionsDlg : public QDialog, public Ui::CAxialAnalysisOptionsDlg {
    Q_OBJECT
  public:
    CAxialAnalysisOptionsDlg(MetaGraph *graph, QWidget *parent = 0);
    void UpdateData(bool value);
    QString m_radius;
    bool m_choice;
    int m_attribute;
    bool m_weighted;
    bool m_rra;
    bool m_local;
    MetaGraph *m_meta_graph;
    void showEvent(QShowEvent *event);

  private slots:
    void OnUpdateRadius();
    void OnWeighted();
    void OnOK();
    void OnCancel() { reject(); }
};
