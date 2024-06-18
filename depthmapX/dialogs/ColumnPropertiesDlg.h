// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include "ui_ColumnPropertiesDlg.h"

#include "salalib/attributetable.h"
#include "salalib/layermanagerimpl.h"

class CColumnPropertiesDlg : public QDialog, public Ui::CColumnPropertiesDlg {
    Q_OBJECT
  public:
    CColumnPropertiesDlg(AttributeTable *table = NULL, LayerManagerImpl *layers = NULL,
                         int col = -1, QWidget *parent = 0);
    AttributeTable *m_table;
    LayerManagerImpl *m_layers;
    int m_col;
    QString m_formula;
    QString m_name;
    QString m_name_text;
    QString m_creator;
    QString m_formula_note;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();

    double sqr(double x) { return x * x; }
};
