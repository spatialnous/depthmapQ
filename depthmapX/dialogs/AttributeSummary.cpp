// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "AttributeSummary.h"

#include "ColumnPropertiesDlg.h"
#include "GraphDoc.h"

CAttributeSummary::CAttributeSummary(QGraphDoc *pDoc, QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_pDoc = pDoc;
}

void CAttributeSummary::OnOK() { accept(); }

void CAttributeSummary::OnDblclkList(int row, int column) {
    if (row != -1) {
        CColumnPropertiesDlg dlg(&(m_pDoc->m_meta_graph->getAttributeTable()),
                                 &(m_pDoc->getLayers()), row);
        dlg.exec();
    }
}

void CAttributeSummary::UpdateData(bool value) {}

void CAttributeSummary::showEvent(QShowEvent *event) {
    const AttributeTable &table = m_pDoc->m_meta_graph->getAttributeTable();

    c_list->setSelectionBehavior(QAbstractItemView::SelectRows);

    c_list->setColumnCount(4);

    QTableWidgetItem *Item;
    Item = new QTableWidgetItem("Attribute");
    c_list->setColumnWidth(0, 220);
    Item->setTextAlignment(Qt::AlignLeft);
    c_list->setHorizontalHeaderItem(0, Item);

    Item = new QTableWidgetItem("Minimum");
    c_list->setColumnWidth(1, 100);
    Item->setTextAlignment(Qt::AlignRight);
    c_list->setHorizontalHeaderItem(1, Item);

    Item = new QTableWidgetItem("Average");
    c_list->setColumnWidth(2, 100);
    Item->setTextAlignment(Qt::AlignRight);
    c_list->setHorizontalHeaderItem(2, Item);

    Item = new QTableWidgetItem("Maximum");
    c_list->setColumnWidth(3, 100);
    Item->setTextAlignment(Qt::AlignRight);
    c_list->setHorizontalHeaderItem(3, Item);

    c_list->clearContents();

    c_list->setRowCount(table.getNumColumns());
    for (int i = 0; i < table.getNumColumns(); i++) {
        const AttributeColumn &column = table.getColumn(i);
        Item = new QTableWidgetItem(QString(column.getName().c_str()));
        Item->setFlags(Qt::NoItemFlags);
        c_list->setRowHeight(i, 20);
        c_list->setItem(i, 0, Item);
        //
        char text[64];
        // Min
        sprintf(text, "%g", column.getStats().min);
        Item = new QTableWidgetItem(QString(text));
        Item->setFlags(Qt::NoItemFlags);
        c_list->setItem(i, 1, Item);
        // Avg
        sprintf(text, "%g", column.getStats().total / table.getNumRows());
        Item = new QTableWidgetItem(QString(text));
        Item->setFlags(Qt::NoItemFlags);
        c_list->setItem(i, 2, Item);
        // Max
        sprintf(text, "%g", column.getStats().max);
        Item = new QTableWidgetItem(QString(text));
        Item->setFlags(Qt::NoItemFlags);
        c_list->setItem(i, 3, Item);
    }
}
