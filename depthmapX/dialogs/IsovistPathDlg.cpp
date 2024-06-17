// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "IsovistPathDlg.h"

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

CIsovistPathDlg::CIsovistPathDlg(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    fov_selection = 0;
    fov_angle = 0.0;
    UpdateData(false);
}

void CIsovistPathDlg::OnOK() {
    UpdateData(true);

    switch (fov_selection) {
    case 0:
        fov_angle = M_PI * 0.5;
        break;
    case 1:
        fov_angle = 2.0 * M_PI / 3.0;
        break;
    case 2:
        fov_angle = M_PI;
        break;
    case 3:
        fov_angle = 2.0 * M_PI;
        break;
    default:
        fov_angle = 2.0 * M_PI;
        break;
    }
    accept();
}

void CIsovistPathDlg::OnCancel() { reject(); }

void CIsovistPathDlg::UpdateData(bool value) {
    if (value) {
        fov_selection = c_fov_selection->currentIndex();
    } else {
        c_fov_selection->setCurrentIndex(fov_selection);
    }
}

void CIsovistPathDlg::showEvent(QShowEvent *event) { UpdateData(false); }
