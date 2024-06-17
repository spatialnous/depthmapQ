// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_InsertColumnDlg.h"
#include <salalib/mgraph.h>
#include <salalib/shapegraph.h>
#include <salalib/shapemap.h>
#include <string>
#include <vector>

class CInsertColumnDlg : public QDialog, public Ui::CInsertColumnDlg {
    Q_OBJECT
  public:
    CInsertColumnDlg(AttributeTable *table = NULL, int col = -1, QWidget *parent = 0);
    bool m_selection_only;
    int m_col;
    std::vector<std::string> m_col_names;
    std::string m_formula_text;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnUseAttribute();
    void OnSelChangeColumnNames();
    void OnDblclkColumnNames(QListWidgetItem *item);
    void OnOK();
    void OnCancel();
};
