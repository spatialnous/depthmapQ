// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_PushDialog.h"
#include <salalib/mgraph.h>
#include <salalib/shapegraph.h>
#include <salalib/shapemap.h>

class CPushDialog : public QDialog, public Ui::CPushDialog {
    Q_OBJECT
  public:
    CPushDialog(std::map<std::pair<int, int>, std::string> &names, QWidget *parent = 0);
    int m_layer_selection;
    QString m_origin_attribute;
    QString m_origin_layer;
    bool m_count_intersections;
    int m_function;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

    std::vector<std::string> m_names;

  private slots:
    void OnOK();
    void OnCancel();
};
