// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "OptionsDlg.h"

#include "mainwindow.h"

#include <QMessageBox>

COptionsDlg::COptionsDlg(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_global = false;
    m_local = false;
    m_radius = tr("");
    m_gates_only = false;
    m_output_type = AnalysisType::NONE;
    m_radius2 = tr("");

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin) {
            m_output_type = mainWin->m_options.output_type;

            m_local = mainWin->m_options.local;
            m_global = mainWin->m_options.global;
            m_gates_only = mainWin->m_options.gates_only;

            if ((int)mainWin->m_options.radius == -1) {
                m_radius = QString("n");
                m_radius2 = QString("n");
            } else if (m_output_type == AnalysisType::VISUAL) {
                char number[2];
                sprintf(number, "%d", (int)mainWin->m_options.radius);
                m_radius = QString(number);
                m_radius2 = tr("n");
            } else {
                char number[32];
                sprintf(number, "%g", mainWin->m_options.radius);
                m_radius = tr("n");
                m_radius2 = QString(number);
            }
            break;
        }
    }
}

void COptionsDlg::OnOutputType(bool value) {
    UpdateData(true);

    if (m_output_type == AnalysisType::VISUAL) {
        c_local->setEnabled(true);
        c_global->setEnabled(true);
        c_radius->setEnabled(true);
    } else {
        c_local->setEnabled(false);
        c_global->setEnabled(false);
        c_radius->setEnabled(false);
        c_radius->setText(tr("n")); // <- essentially, undo changes
    }

    if (m_output_type == AnalysisType::METRIC) {
        c_radius2->setEnabled(true);
    } else {
        c_radius2->setText(tr("n")); // <- essentially, undo changes
        c_radius2->setEnabled(false);
    }
}

void COptionsDlg::OnUpdateRadius(QString text) {
    if (text.length()) {
        if (!text.toInt() && text != tr("n")) {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("The radius must either be n or number in range 1-99"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            c_radius->setText(tr("n"));
        }
    }
}

void COptionsDlg::OnUpdateRadius2(QString text) {
    if (text.length()) {
        if (text.toDouble() == 0.0 && text != tr("n")) {
            QMessageBox::warning(this, tr("Warning"),
                                 tr("The radius must either be n or a positive number"),
                                 QMessageBox::Ok, QMessageBox::Ok);
            c_radius2->setText(tr("n"));
        }
    }
}

void COptionsDlg::OnOK() {
    UpdateData(true);

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin) {
            mainWin->m_options.local = m_local;
            mainWin->m_options.global = m_global;
            mainWin->m_options.output_type = m_output_type;
            mainWin->m_options.gates_only = m_gates_only;
            mainWin->m_options.gatelayer = c_layer_selector->currentIndex() - 1;

            if (m_output_type == AnalysisType::VISUAL) {
                if (m_radius.compare(tr("n")) == 0) { // 0 means identical
                    mainWin->m_options.radius = -1.0;
                } else {
                    mainWin->m_options.radius = (double)m_radius.toInt();
                    if (mainWin->m_options.radius <= 0.0) {
                        QMessageBox::warning(
                            this, tr("Warning"),
                            tr("The radius must either be n or a number in the range 1-99"),
                            QMessageBox::Ok, QMessageBox::Ok);
                        return;
                    }
                }
            } else {
                if (m_radius2.compare(tr("n")) == 0) { // 0 means identical
                    mainWin->m_options.radius = -1.0;
                } else {
                    mainWin->m_options.radius = m_radius2.toDouble();
                    if (mainWin->m_options.radius <= 0.0) {
                        QMessageBox::warning(this, tr("Warning"),
                                             tr("The radius must either be n or a positive number"),
                                             QMessageBox::Ok, QMessageBox::Ok);
                        return;
                    }
                }
            }
            break;
        }
    }
    accept();
}

void COptionsDlg::OnCancel() { reject(); }

void COptionsDlg::UpdateData(bool value) {
    if (value) {
        if (c_global->checkState())
            m_global = true;
        else
            m_global = false;

        if (c_local->checkState())
            m_local = true;
        else
            m_local = false;

        m_radius = c_radius->text();

        if (c_output_type->isChecked())
            m_output_type = AnalysisType::ISOVIST;
        else if (c_radio1->isChecked())
            m_output_type = AnalysisType::VISUAL;
        else if (c_radio2->isChecked())
            m_output_type = AnalysisType::METRIC;
        else if (c_radio3->isChecked())
            m_output_type = AnalysisType::ANGULAR;
        else if (c_radio4->isChecked())
            m_output_type = AnalysisType::THRU_VISION;
        else
            m_output_type = AnalysisType::NONE;
        m_radius2 = c_radius2->text();
    } else {
        if (m_global)
            c_global->setCheckState(Qt::Checked);
        else
            c_global->setCheckState(Qt::Unchecked);

        if (m_local)
            c_local->setCheckState(Qt::Checked);
        else
            c_local->setCheckState(Qt::Unchecked);

        c_radius->setText(m_radius);

        switch (m_output_type) {
        case AnalysisType::ISOVIST:
            c_output_type->setChecked(true);
            break;
        case AnalysisType::VISUAL:
            c_radio1->setChecked(true);
            break;
        case AnalysisType::METRIC:
            c_radio2->setChecked(true);
            break;
        case AnalysisType::ANGULAR:
            c_radio3->setChecked(true);
            break;
        case AnalysisType::THRU_VISION:
            c_radio4->setChecked(true);
            break;
        default:
            break;
        }
        c_radius2->setText(m_radius2);
    }
}

void COptionsDlg::showEvent(QShowEvent *event) {
    for (size_t i = 0; i < m_layer_names.size(); i++) {
        c_layer_selector->addItem(QString(m_layer_names[i].c_str()));
    }
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin) {
            c_layer_selector->setCurrentIndex(mainWin->m_options.gatelayer + 1);
            break;
        }
    }

    OnOutputType(false);

    UpdateData(false);
}
