// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ui_SegmentTulipLeafChoiceDlg.h"

#include "dminterface/metagraphdm.hpp"

#include "salalib/radiustype.hpp"

class CSegmentTulipLeafChoiceDlg : public QDialog, public Ui::CSegmentTulipLeafChoiceDlg {
    Q_OBJECT
  public:
    CSegmentTulipLeafChoiceDlg(MetaGraphDM *graph = NULL, QWidget *parent = 0);
    QString m_radius;
    int m_tulip_bins;
    RadiusType m_radius_type;
    bool m_weighted;
    int m_attribute;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

    MetaGraphDM *m_meta_graph;

  private slots:
    void OnAnalysisTulip(bool);
    void OnUpdateRadius(QString);
    void OnWeighted(bool);
    void OnOK();
    void OnCancel();
};
