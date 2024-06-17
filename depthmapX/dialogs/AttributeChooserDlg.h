// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_AttributeChooserDlg.h"
#include <salalib/mgraph.h>
#include <salalib/shapegraph.h>
#include <salalib/shapemap.h>

class CAttributeChooserDlg : public QDialog, public Ui::CAttributeChooserDlg {
    Q_OBJECT
  public:
    CAttributeChooserDlg(AttributeTable &table, QWidget *parent = 0);
    int m_attribute;
    QString m_text;
    AttributeTable *m_table;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnCancel();
};
