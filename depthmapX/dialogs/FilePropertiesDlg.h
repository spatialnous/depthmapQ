// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_FilePropertiesDlg.h"

class CFilePropertiesDlg : public QDialog, public Ui::CFilePropertiesDlg {
    Q_OBJECT
  public:
    CFilePropertiesDlg(QWidget *parent = 0);
    QString m_author;
    QString m_create_date;
    QString m_create_program;
    QString m_description;
    QString m_location;
    QString m_organization;
    QString m_title;
    QString m_file_version;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnOK();
    void OnCancel();
};
