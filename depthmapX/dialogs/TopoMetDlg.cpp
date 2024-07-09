// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "TopoMetDlg.h"

#include <QMessageBox>

CTopoMetDlg::CTopoMetDlg(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_analysisType = AnalysisType::ISOVIST;
    m_selected_only = false;
    m_radius = tr("n");

    UpdateData(false);
}

void CTopoMetDlg::OnOK() {
    UpdateData(true);

    /*m_radius.TrimLeft(' ');
    m_radius.TrimRight(' ');*/

    // my own validate on the radius (note: on fail to convert, atoi returns 0)
    if (m_radius.isEmpty() || (m_radius.indexOf("n") == -1 && m_radius.indexOf("N") == -1 &&
                               m_radius.indexOf("1") == -1 && m_radius.indexOf("2") == -1 &&
                               m_radius.indexOf("3") == -1 && m_radius.indexOf("4") == -1 &&
                               m_radius.indexOf("5") == -1 && m_radius.indexOf("6") == -1 &&
                               m_radius.indexOf("7") == -1 && m_radius.indexOf("8") == -1 &&
                               m_radius.indexOf("9") == -1)) {
        QMessageBox::warning(this, tr("Warning"), tr("The radius must either be numeric or 'n'"),
                             QMessageBox::Ok, QMessageBox::Ok);
        m_radius = tr("n");
        UpdateData(false);
        return;
    }

    if (m_radius == "n" || m_radius == "N") {
        m_dradius = -1.0;
    } else {
        m_dradius = m_radius.toDouble();
        if (m_dradius <= 0.0) {
            QMessageBox::warning(
                this, tr("Warning"),
                tr("The radius must either be 'n' or a number in the range 0.0 to infinity"),
                QMessageBox::Ok, QMessageBox::Ok);
            return;
        }
    }

    accept();
}

void CTopoMetDlg::UpdateData(bool value) {
    if (value) {
        if (c_topological->isChecked())
            m_analysisType = AnalysisType::ISOVIST;
        else if (radioButton->isChecked())
            m_analysisType = AnalysisType::VISUAL;
        else
            m_analysisType = AnalysisType::NONE;
        m_radius = c_radius->text();
        if (checkBox->checkState())
            m_selected_only = true;
        else
            m_selected_only = false;
    } else {
        switch (m_analysisType) {
        case AnalysisType::ISOVIST:
            c_topological->setChecked(true);
            break;
        case AnalysisType::VISUAL:
            radioButton->setChecked(true);
            break;
        default:
            break;
        }
        c_radius->setText(m_radius);
        if (m_selected_only)
            checkBox->setCheckState(Qt::Checked);
        else
            checkBox->setCheckState(Qt::Unchecked);
    }
}

void CTopoMetDlg::showEvent(QShowEvent *event) { UpdateData(false); }
