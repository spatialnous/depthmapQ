// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "depthmapX/ui_ColourScaleDlg.h"

#include "depthmapX/graphdoc.h"

class CColourScaleDlg : public QDialog, public Ui::CColourScaleDlg {
    Q_OBJECT
  public:
    CColourScaleDlg(QWidget *parent = 0);
    QGraphDoc *m_viewDoc;
    QString m_blue;
    QString m_red;
    int m_color;

    bool m_docked;

    double m_display_min;
    double m_display_max;
    double GetActualValue(double sliderpos);
    float GetNormValue(double actualval);

    QBrush m_red_brush;
    QBrush m_blue_brush;
    DisplayParams m_displayparams;

    void MyUpdateData(bool dir, bool apply_to_all);
    void Clear();
    void Fill();
    bool m_show_lines;
    bool m_show_fill;
    bool m_show_centroids;
    std::vector<int> m_color_type_map;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);
    void OnFocusGraph(QGraphDoc *pDoc, int lParam);

  private slots:
    void OnChangeBlueValue();
    void OnChangeRedValue();
    void OnReleasedRedSlider(int);
    void OnReleasedBlueSlider(int);
    void OnSelchangeColor(int);
    void OnBnClickedShowLines(bool);
    void OnBnClickedShowFill(bool);
    void OnBnClickedShowCentroids(bool);
    void OnBnClickedApplytoall();
    void OnOK();
    void OnCancel();
};
