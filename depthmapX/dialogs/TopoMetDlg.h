// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_TopoMetDlg.h"

class CTopoMetDlg : public QDialog, public Ui::CTopoMetDlg {
    Q_OBJECT
  private:
    enum { TOPOMET_METHOD_TOPOLOGICAL = 0, TOPOMET_METHOD_METRIC = 1 };

  public:
    CTopoMetDlg(QWidget *parent = 0);
    int m_topological;
    QString m_radius;
    double m_dradius;
    bool m_selected_only;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

    bool isAnalysisTopological() { return m_topological == TOPOMET_METHOD_TOPOLOGICAL; }

  private slots:
    void OnOK();
};
