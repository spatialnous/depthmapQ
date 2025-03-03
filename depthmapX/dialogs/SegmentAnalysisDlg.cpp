// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "SegmentAnalysisDlg.hpp"

#include "mainwindow.hpp"

#include <QMessageBox>

CSegmentAnalysisDlg::CSegmentAnalysisDlg(MetaGraphDX *graph, QWidget *parent) : QDialog(parent) {
    setupUi(this);
    m_analysis_type = -1;
    m_radius = tr("");
    m_tulip_bins = 0;
    m_radius_type = RadiusType::NONE;
    m_choice = false;
    m_weighted = false;
    m_attribute = -1;

    m_meta_graph = graph;
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin) {
            if (mainWin->m_options.tulipBins == 0) {
                m_analysis_type = 1;
                m_tulip_bins = 1024;
            } else {
                m_analysis_type = 0;
                m_tulip_bins = mainWin->m_options.tulipBins;
            }
            m_choice = mainWin->m_options.choice;
            m_radius_type = mainWin->m_options.radiusType;
            if ((int)mainWin->m_options.radius == -1) {
                m_radius = QString("n");
            } else {
                char number[32];
                sprintf(number, "%g", mainWin->m_options.radius);
                m_radius = QString(number);
            }
            if (mainWin->m_options.weightedMeasureCol == -1) {
                m_weighted = false;
                m_attribute = -1;
            } else {
                m_weighted = true;
                m_attribute = 0;
            }
            break;
        }
    }
}

void CSegmentAnalysisDlg::OnAnalysisType(bool value) {
    UpdateData(true);
    c_tulip_bins->setEnabled(true);
    c_choice->setEnabled(true);
    c_radius_type->setEnabled(true);
    radioButton->setEnabled(true);
    c_weighted->setEnabled(true);
    c_attribute->setEnabled(false);
}

void CSegmentAnalysisDlg::OnAnalysisTulip(bool value) {
    // actually, not tulip -- they're switched, this is on analyse angular!!
    UpdateData(true);
    m_choice = false;
    m_radius_type = RadiusType::ANGULAR;
    m_analysis_type = 1;
    m_weighted = false;
    m_attribute = -1;
    UpdateData(false);
    c_tulip_bins->setEnabled(false);
    c_choice->setEnabled(false);
    c_radius_type->setEnabled(false);
    radioButton->setEnabled(false);
    c_weighted->setEnabled(false);
    c_attribute->setEnabled(false);
}

void CSegmentAnalysisDlg::OnUpdateRadius(QString text) {
    if (!text.isEmpty() && text.indexOf("n") == -1 && text.indexOf("N") == -1 &&
        text.indexOf("1") == -1 && text.indexOf("2") == -1 && text.indexOf("3") == -1 &&
        text.indexOf("4") == -1 && text.indexOf("5") == -1 && text.indexOf("6") == -1 &&
        text.indexOf("7") == -1 && text.indexOf("8") == -1 && text.indexOf("9") == -1) {
        QMessageBox::warning(
            this, tr("Warning"),
            tr("The radius must either be numeric or 'n'\nAlternatively, for multiple radii, type "
               "a list of comma separated numeric radii (you can include 'n')"),
            QMessageBox::Ok, QMessageBox::Ok);
        c_radius->setText(tr("n"));
        c_radius->setFocus(Qt::OtherFocusReason);
    }
}

void CSegmentAnalysisDlg::OnWeighted(bool value) {
    if (value) {
        UpdateData(true);
        c_attribute->setEnabled(true);
        m_attribute = 0;
        UpdateData(false);
    } else {
        UpdateData(true);
        c_attribute->setEnabled(false);
        m_attribute = -1;
        UpdateData(false);
    }
}

void CSegmentAnalysisDlg::OnOK() {
    UpdateData(true);

    // my own validate on the radius (note: on fail to convert, atoi returns 0)
    if (m_radius.isEmpty() || (m_radius.indexOf("n") == -1 && m_radius.indexOf("N") == -1 &&
                               m_radius.indexOf("1") == -1 && m_radius.indexOf("2") == -1 &&
                               m_radius.indexOf("3") == -1 && m_radius.indexOf("4") == -1 &&
                               m_radius.indexOf("5") == -1 && m_radius.indexOf("6") == -1 &&
                               m_radius.indexOf("7") == -1 && m_radius.indexOf("8") == -1 &&
                               m_radius.indexOf("9") == -1)) {
        QMessageBox::warning(
            this, tr("Warning"),
            tr("The radius must either be numeric or 'n'\nAlternatively, for multiple radii, type "
               "a list of comma separated numeric radii (you can include 'n')"),
            QMessageBox::Ok, QMessageBox::Ok);
        m_radius = tr("n");
        UpdateData(false);
        c_radius->setFocus(Qt::OtherFocusReason);
        return;
    }
    // now parse radius list:
    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
        if (mainWin) {
            mainWin->m_options.radiusSet.clear();
            QString curr_radius;
            int curr_comma = -1, last_comma = 0;
            bool add_rn = false;
            do {
                curr_comma = m_radius.indexOf(',', last_comma);
                if (curr_comma != -1) {
                    curr_radius = m_radius.mid(last_comma, curr_comma - last_comma);
                    last_comma = curr_comma + 1;
                } else {
                    curr_radius = m_radius.mid(last_comma);
                }
                /*curr_radius.TrimLeft(' ');
                curr_radius.TrimRight(' ');*/
                if (!curr_radius.isEmpty()) {
                    if (curr_radius == "n" || curr_radius == "N") {
                        add_rn = true;
                    } else {
                        double radius;
                        if (m_radius_type == RadiusType::TOPOLOGICAL) {
                            radius = curr_radius.toDouble();
                            if (radius < 1 || radius > 99) {
                                QMessageBox::warning(this, tr("Warning"),
                                                     tr("Each radius in the list must either be "
                                                        "'n' or a number in the range 1-99"),
                                                     QMessageBox::Ok, QMessageBox::Ok);
                                c_radius->setFocus(Qt::OtherFocusReason);
                                return;
                            }
                        } else {
                            radius = curr_radius.toDouble();
                            if (radius <= 0.0) {
                                QMessageBox::warning(
                                    this, tr("Warning"),
                                    tr("Each radius in the list must either be 'n' or a number in "
                                       "the range 0.0 to infinity"),
                                    QMessageBox::Ok, QMessageBox::Ok);
                                c_radius->setFocus(Qt::OtherFocusReason);
                                return;
                            }
                        }
                        mainWin->m_options.radiusSet.insert(double(radius));
                    }
                }
            } while (curr_comma != -1);
            if (mainWin->m_options.radiusSet.size() == 0 || add_rn) {
                mainWin->m_options.radiusSet.insert(-1);
            }

            if (m_tulip_bins % 2 != 0) {
                QMessageBox::warning(this, tr("Warning"),
                                     tr("The number of tulip bins must be an even number"),
                                     QMessageBox::Ok, QMessageBox::Ok);
                return;
            }

            mainWin->m_options.choice = m_choice;
            mainWin->m_options.radiusType = m_radius_type;

            if (m_analysis_type == 1) {
                mainWin->m_options.tulipBins = 0;
            } else {
                mainWin->m_options.tulipBins = m_tulip_bins;
            }

            // attributes:
            if (!m_weighted) {
                mainWin->m_options.weightedMeasureCol = -1;
            } else {
                mainWin->m_options.weightedMeasureCol = m_attribute;
            }
            break;
        }
    }
    accept();
}

void CSegmentAnalysisDlg::OnCancel() { reject(); }

void CSegmentAnalysisDlg::UpdateData(bool value) {
    if (value) {
        if (c_analysis_type->isChecked()) {
            c_radio2->setChecked(false);
            m_analysis_type = 0;
        } else if (c_radio2->isChecked()) {
            m_analysis_type = 1;
            c_analysis_type->setChecked(false);
        } else
            m_analysis_type = -1;

        m_radius = c_radius->text();
        m_tulip_bins = c_tulip_bins->text().toInt();

        if (c_radius_type->isChecked())
            m_radius_type = RadiusType::TOPOLOGICAL;
        else if (radioButton->isChecked())
            m_radius_type = RadiusType::METRIC;
        else if (c_radio3->isChecked())
            m_radius_type = RadiusType::ANGULAR;
        else
            m_radius_type = RadiusType::NONE;

        if (c_choice->checkState())
            m_choice = true;
        else
            m_choice = false;
        if (c_weighted->checkState())
            m_weighted = true;
        else
            m_weighted = false;
        m_attribute = c_attribute->currentIndex();
    } else {
        switch (m_analysis_type) {
        case 0:
            c_analysis_type->setChecked(true);
            break;
        case 1:
            c_analysis_type->setChecked(false);
            break;
        default:
            break;
        }
        c_radius->setText(m_radius);
        c_tulip_bins->setText(QString("%1").arg(m_tulip_bins));
        switch (m_radius_type) {
        case RadiusType::TOPOLOGICAL:
            c_radius_type->setChecked(true);
            break;
        case RadiusType::METRIC:
            radioButton->setChecked(true);
            break;
        case RadiusType::ANGULAR:
            c_radio3->setChecked(true);
            break;
        case RadiusType::NONE:
            break;
        }

        if (m_choice)
            c_choice->setCheckState(Qt::Checked);
        else
            c_choice->setCheckState(Qt::Unchecked);

        if (m_weighted)
            c_weighted->setCheckState(Qt::Checked);
        else
            c_weighted->setCheckState(Qt::Unchecked);
        c_attribute->setCurrentIndex(m_attribute);
    }
}

void CSegmentAnalysisDlg::showEvent(QShowEvent *event) {
    const auto &map = m_meta_graph->getDisplayedShapeGraph();
    const auto &table = map.getAttributeTable();
    for (int i = 0; i < table.getNumColumns(); i++) {
        c_attribute->addItem(QString(table.getColumnName(i).c_str()));
    }

    if (m_analysis_type == 1) {
        m_choice = false;
        m_radius_type = RadiusType::ANGULAR;
        m_weighted = false;
        UpdateData(false);
        c_tulip_bins->setEnabled(false);
        c_choice->setEnabled(false);
        c_radius_type->setEnabled(false);
        radioButton->setEnabled(false);
        c_weighted->setEnabled(false);
        c_attribute->setEnabled(false);
    } else {
        c_tulip_bins->setEnabled(true);
        c_choice->setEnabled(true);
        c_radius_type->setEnabled(true);
        radioButton->setEnabled(true);
        c_weighted->setEnabled(true);
        if (m_weighted) {
            c_attribute->setEnabled(true);
            m_attribute = 0;
            UpdateData(false);
        } else {
            m_attribute = -1;
            c_attribute->setEnabled(false);
        }
    }

    UpdateData(false);
}
