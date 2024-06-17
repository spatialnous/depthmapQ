// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_LayerChooserDlg.h"
#include <salalib/mgraph.h>
#include <salalib/shapegraph.h>
#include <salalib/shapemap.h>

class CLayerChooserDlg : public QDialog, public Ui::CLayerChooserDlg {
    Q_OBJECT
  public:
    CLayerChooserDlg(const std::vector<std::string> &names = std::vector<std::string>(),
                     QWidget *parent = 0);
    QString m_text;
    int m_layer;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

    const std::vector<std::string> &m_names;

  private slots:
    void OnOK();
    void OnCancel();
};
