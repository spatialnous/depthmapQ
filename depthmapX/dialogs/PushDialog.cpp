// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "PushDialog.h"

CPushDialog::CPushDialog(std::map<std::pair<int, int>, std::string> &names, QWidget *parent)
    : QDialog(parent) {
    setupUi(this);
    m_layer_selection = -1;
    m_origin_attribute = tr("");
    m_origin_layer = tr("");
    m_count_intersections = false;
    m_function = PushValues::Func::MAX;

    for (auto name : names) {
        m_names.push_back(name.second);
    }
}

void CPushDialog::OnOK() {
    UpdateData(true);
    accept();
}

void CPushDialog::OnCancel() { reject(); }

void CPushDialog::UpdateData(bool value) {
    if (value) {
        m_layer_selection = c_layer_selector->currentIndex();
        m_origin_attribute = c_origin_attribute->text();
        m_origin_layer = c_origin_layer->text();
        if (c_count_intersections->checkState())
            m_count_intersections = true;
        else
            m_count_intersections = false;
        switch (c_function->currentIndex()) {
        case 0:
            m_function = PushValues::Func::MAX;
            break;
        case 1:
            m_function = PushValues::Func::MIN;
            break;
        case 2:
            m_function = PushValues::Func::AVG;
            break;
        case 3:
            m_function = PushValues::Func::TOT;
            break;
        case -1:
            m_function = PushValues::Func::NONE;
            break;
        }

    } else {
        c_layer_selector->setCurrentIndex(m_layer_selection);
        c_origin_attribute->setText(m_origin_attribute);
        c_origin_layer->setText(m_origin_layer);
        if (m_count_intersections)
            c_count_intersections->setCheckState(Qt::Checked);
        else
            c_count_intersections->setCheckState(Qt::Unchecked);

        switch (m_function) {
        case PushValues::Func::MAX:
            c_function->setCurrentIndex(0);
            break;
        case PushValues::Func::MIN:
            c_function->setCurrentIndex(1);
            break;
        case PushValues::Func::AVG:
            c_function->setCurrentIndex(2);
            break;
        case PushValues::Func::TOT:
            c_function->setCurrentIndex(3);
            break;
        case PushValues::Func::NONE:
            c_function->setCurrentIndex(-1);
            break;
        }
    }
}

void CPushDialog::showEvent(QShowEvent *event) {
    for (size_t i = 0; i < m_names.size(); i++) {
        c_layer_selector->addItem(QString(m_names[i].c_str()));
    }
    c_layer_selector->setCurrentIndex(0);

    UpdateData(false);
}
