// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ColumnPropertiesDlg.h"

#include "salalib/genlib/stringutils.h"

CColumnPropertiesDlg::CColumnPropertiesDlg(AttributeTable *table, LayerManagerImpl *layers,
                                           std::optional<const std::set<int>> selSet, int col,
                                           QWidget *parent)
    : QDialog(parent), m_table(table), m_layers(layers), m_selSet(selSet), m_col(col) {
    setupUi(this);
    m_formula = tr("");
    m_name = tr("");
    m_name_text = tr("");
    m_creator = tr("");
    m_formula_note = tr("");

    AttributeColumn &column = m_table->getColumn(m_col);
    m_name = column.getName().c_str();
    m_formula = column.getFormula().c_str();

    if (!column.isLocked()) {
        m_name_text = "Name";
    } else {
        m_name_text = "Name (column locked and cannot be edited)";
    }

    if (m_formula.isEmpty()) {
        // m_formula_note.Empty();
    } else {
        m_formula_note = tr("Note: the formula may have been applied to a subset of objects");
    }

    UpdateData(false);
}

void CColumnPropertiesDlg::OnOK() {
    UpdateData(true);
    accept();
}

void CColumnPropertiesDlg::UpdateData(bool value) {
    std::vector<QString> rows;
    std::vector<double> summary_all;
    std::vector<double> summary_sel;

    rows.push_back(tr("Average"));
    rows.push_back(tr("Minimum"));
    rows.push_back(tr("Maximum"));
    rows.push_back(tr("Std Dev"));
    rows.push_back(tr("Count"));

    int i;
    for (i = 0; i < 15; i++) {
        if (i == 1 || i == 2) {
            // minimum and maximum
            summary_all.push_back(-1.0);
            summary_sel.push_back(-1.0);
        } else {
            summary_all.push_back(0.0);
            summary_sel.push_back(0.0);
        }
    }

    for (auto iter = m_table->begin(); iter != m_table->end(); iter++) {
        auto &row = iter->getRow();
        double val = row.getValue(m_col);
        if (val != -1.0 && isObjectVisible(*m_layers, iter->getRow())) {
            summary_all[0] += val;
            summary_all[4] += 1.0;
            if (summary_all[1] == -1.0 || val < summary_all[1]) {
                summary_all[1] = val;
            }
            if (summary_all[2] == -1.0 || val > summary_all[2]) {
                summary_all[2] = val;
            }
            if (m_selSet.has_value() && (m_selSet->find(iter->getKey().value) != m_selSet->end())) {
                summary_sel[0] += val;
                summary_sel[4] += 1.0;
                if (summary_sel[1] == -1.0 || val < summary_sel[1]) {
                    summary_sel[1] = val;
                }
                if (summary_sel[2] == -1.0 || val > summary_sel[2]) {
                    summary_sel[2] = val;
                }
            }
        }
    }

    bool freqrows = false;
    double unit;
    if (summary_all[1] != -1.0 && summary_all[2] != -1.0 && summary_all[1] != summary_all[2]) {
        freqrows = true;
        unit = (summary_all[2] - summary_all[1]) / 10.0;
        for (int i = 0; i < 10; i++) {
            std::string name;
            if (i == 0) {
                name = dXstring::formatString(summary_all[1] + unit, "< %f");
            } else if (i == 9) {
                name = dXstring::formatString(summary_all[2] - unit, "> %f");
            } else {
                name = dXstring::formatString(summary_all[1] + unit * i, "%f") + " to " +
                       dXstring::formatString(summary_all[1] + unit * (i + 1), "%f");
            }
            // Unicode conversion a bit of a mess here AT (01.02.11)
            rows.push_back(QString(name.c_str()));
        }
    }

    if (summary_all[4] != 0) {
        summary_all[0] /= summary_all[4];
    }
    if (summary_sel[4] != 0) {
        summary_sel[0] /= summary_sel[4];
    }

    // count of things rows: just for visible at the moment

    double var_all = 0.0;
    double var_sel = 0.0;
    for (auto iter = m_table->begin(); iter != m_table->end(); iter++) {
        auto &row = iter->getRow();
        double val = row.getValue(m_col);
        if (val != -1.0 && isObjectVisible(*m_layers, iter->getRow())) {
            var_all += sqr(val - summary_all[0]);
            if (freqrows) {
                int pos = floor((val - summary_all[1]) / unit);
                if (pos == 10)
                    pos = 9; // irritating exactly equal to max
                summary_all[5 + pos] += 1;
            }
            if (m_selSet.has_value() && (m_selSet->find(iter->getKey().value) != m_selSet->end())) {
                var_sel += sqr(val - summary_sel[0]);
                if (freqrows) {
                    // note: must use summary_all even on selected to make difference
                    int pos = floor((val - summary_all[1]) / unit);
                    if (pos == 10)
                        pos = 9; // irritating exactly equal to max
                    summary_sel[5 + pos] += 1;
                }
            }
        }
    }

    if (summary_all[4] != 0) {
        summary_all[3] = sqrt(var_all / summary_all[4]);
    }
    if (summary_sel[4] != 0) {
        summary_sel[3] = sqrt(var_sel / summary_sel[4]);
    }

    c_summary->setSelectionBehavior(QAbstractItemView::SelectRows);
    c_summary->setColumnCount(3);

    QTableWidgetItem *Item;
    Item = new QTableWidgetItem("Value");
    c_summary->setColumnWidth(0, 100);
    Item->setTextAlignment(Qt::AlignLeft);
    c_summary->setHorizontalHeaderItem(0, Item);

    Item = new QTableWidgetItem("Attribute");
    c_summary->setColumnWidth(1, 100);
    Item->setTextAlignment(Qt::AlignRight);
    c_summary->setHorizontalHeaderItem(1, Item);

    Item = new QTableWidgetItem("Selection");
    c_summary->setColumnWidth(2, 100);
    Item->setTextAlignment(Qt::AlignRight);
    c_summary->setHorizontalHeaderItem(2, Item);

    c_summary->clearContents();

    c_summary->setRowCount(15);
    for (i = 0; i < 15; i++) {
        if (i == 5 && !freqrows) {
            break;
        }
        Item = new QTableWidgetItem(rows[i]);
        Item->setFlags(Qt::NoItemFlags);
        c_summary->setRowHeight(i, 20);
        c_summary->setItem(i, 0, Item);
        //
        char text[64];
        // All
        if (i == 4 || summary_all[4] != 0) {
            sprintf(text, "%g", summary_all[i]);
        } else {
            strcpy(text, "No Value");
        }
        Item = new QTableWidgetItem(QString(text));
        Item->setFlags(Qt::NoItemFlags);
        c_summary->setItem(i, 1, Item);
        // Sel
        if (i == 4 || summary_sel[4] != 0) {
            sprintf(text, "%g", summary_sel[i]);
        } else {
            strcpy(text, "No Value");
        }
        Item = new QTableWidgetItem(QString(text));
        Item->setFlags(Qt::NoItemFlags);
        c_summary->setItem(i, 2, Item);
    }
    if (value) {
        m_formula = c_formula->toPlainText();
        m_name = c_name->text();
        m_name_text = c_name_text->text();
        m_formula_note = c_formula_note->text();
    } else {
        c_formula->setPlainText(m_formula);
        c_name->setText(m_name);
        c_name_text->setText(m_name_text);
        c_formula_note->setText(m_formula_note);
    }
}

void CColumnPropertiesDlg::showEvent(QShowEvent *event) { UpdateData(false); }
