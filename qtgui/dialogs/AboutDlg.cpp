// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "AboutDlg.hpp"

#include "version.hpp"
#include "version_defs.hpp"

CAboutDlg::CAboutDlg(QWidget *parent) : QDialog(parent) {
    setupUi(this);
    QString m_version_info;
    m_version_info = QString(tr("Version %1.%2.%3 (%4, %5)"))
                         .arg(DEPTHMAPX_MAJOR_VERSION)
                         .arg(DEPTHMAPX_MINOR_VERSION)
                         .arg(DEPTHMAPX_REVISION_VERSION)
                         .arg(APP_GIT_BRANCH)
                         .arg(APP_GIT_COMMIT);
    QString m_copyright;
    m_copyright = QString(tr("(C) 2000-2010\tUniversity College London,\n"
                             "\t\tAlasdair Turner,\n"
                             "\t\tEva Friedrich\n"
                             "(C) 2011-2014\tTasos Varoudis\n"
                             "(C) 2017-2020\tChristian Sailer,\n"
                             "\t\tPetros Koutsolampros\n"
                             "(C) 2020-2025\tPetros Koutsolampros"));
    QString m_agreement;
    m_agreement = QString(tr(
        "This program is free software: you can redistribute it and/or modify it under the terms "
        "of the GNU General Public License as published by the Free Software Foundation, either "
        "version 3 of the License, or (at your option) any later "
        "version.\x0D\x0D\x0A\x0D\x0D\x0AThis program is distributed in the hope that it will be "
        "useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or "
        "FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more "
        "details.\x0D\x0D\x0A\x0D\x0D\x0AYou should have received a copy of the GNU General Public "
        "License along with this program.  If not, see <http://www.gnu.org/licenses/>."));

    c_version_info->setText(m_version_info);
    c_copyright->setText(m_copyright);
    c_agreement->setText(m_agreement);
}

void CAboutDlg::OnOK() { accept(); }
