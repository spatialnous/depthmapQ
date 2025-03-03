// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "salalib/analysistype.h"
#include "ui_TopoMetDlg.h"

class CTopoMetDlg : public QDialog, public Ui::CTopoMetDlg {
    Q_OBJECT

  public:
    CTopoMetDlg(QWidget *parent = 0);
    AnalysisType m_analysisType;
    QString m_radius;
    double m_dradius;
    bool m_selected_only;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

    // TODO: P.K. Originally the "analysis type" function here worked with indices,
    // never actually matching them to a universal analysis type. With this
    // global enum we keep the original index-based enum names until corrected
    bool isAnalysisTopological() { return m_analysisType == AnalysisType::ISOVIST; }

  private slots:
    void OnOK();
};
