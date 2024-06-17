// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ui_MakeLayerDlg.h"
#include <salalib/mgraph.h>
#include <salalib/shapegraph.h>
#include <salalib/shapemap.h>

enum {
    MAKELAYER_DRAWING = 1,
    MAKELAYER_DATA = 2,
    MAKELAYER_AXIAL = 4,
    MAKELAYER_CONVEX = 8,
    MAKELAYER_GENERIC = 16,
    MAKELAYER_SEGMENT = 32
};

class CMakeLayerDlg : public QDialog, public Ui::CMakeLayerDlg {
    Q_OBJECT
  public:
    CMakeLayerDlg(QWidget *parent = 0);
    bool m_remove_stubs;
    bool m_push_values;
    int m_percentage;
    QString m_origin;
    QString m_layer_name;

    int m_mapin;
    int m_mapout;

    std::vector<int> m_lookup;
    bool m_keeporiginal;
    void UpdateData(bool value);
    void showEvent(QShowEvent *event);

  private slots:
    void OnSelchangeLayerType(int);
    void OnRemoveStubs(bool);
    void OnOK();
    void OnCancel();
};
