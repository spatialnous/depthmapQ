// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ui_AgentAnalysisDlg.h"

class CAgentAnalysisDlg : public QDialog, public Ui::CAgentAnalysisDlg {
    Q_OBJECT
  public:
    CAgentAnalysisDlg(QWidget *parent = 0);
    int m_release_location;
    int m_fov;
    int m_frames;
    double m_release_rate;
    int m_steps;
    int m_timesteps;
    int m_occlusion;
    bool m_record_trails;
    int m_trail_count;
    //}}AFX_DATA

    int m_gatelayer;
    std::vector<std::string> m_names;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnCancel();
};
