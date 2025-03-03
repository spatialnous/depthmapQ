// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
// SPDX-FileCopyrightText: 2019 Petros Koutsolampros
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "graphdoc.h"
#include "settings.h"

#include <QTableWidget>

class QEvent;
class QTableWidgetItem;

class TableView : public QTableWidget {
    Q_OBJECT

  public:
    TableView(Settings &settings, QWidget *parent = 0, QGraphDoc *p = 0);
    QSize sizeHint() const;

    int m_column_count;
    int m_row_count;
    int m_from;
    int m_curr_row;
    QWidget *m_mainWindow;
    QGraphDoc *pDoc;
    bool m_protect_edit;
    void RedoTable();

  private slots:
    void itemChanged(QTableWidgetItem *item);
    void itemEditChanged(QTableWidgetItem *);
    void colum_Sort(int sort);

  protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void scrollContentsBy(int dx, int dy);
    void focusInEvent(QFocusEvent *e);

  private:
    void PrepareCache(int to);

    bool m_custom;
    QSize m_initialSize;
    bool m_updating = false;
};
