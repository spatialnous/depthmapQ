// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_AttributeSummary.h"

class QGraphDoc;

class CAttributeSummary : public QDialog, public Ui::CAttributeSummary {
    Q_OBJECT
  public:
    CAttributeSummary(QGraphDoc *pDoc, QWidget *parent = 0);
    QGraphDoc *m_pDoc;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnDblclkList(int row, int column);
};
