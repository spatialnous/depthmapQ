// SPDX-FileCopyrightText: 2011-2012 Tasos Varoudis
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef indexWidget_H
#define indexWidget_H

#include <QTreeWidget>

QT_BEGIN_NAMESPACE

class QEvent;
class QTreeWidgetItem;

class IndexWidget : public QTreeWidget {
    Q_OBJECT
  private:
    enum Column { MAP = 0, EDITABLE = 1 };

  public:
    IndexWidget(QWidget *parent = 0);

    QString m_mapColumn = "Map";
    QString m_editableColumn = "Editable";

    void setItemVisibility(QTreeWidgetItem *item, Qt::CheckState checkState) {
        item->setCheckState(Column::MAP, checkState);
    }
    void setItemEditability(QTreeWidgetItem *item, Qt::CheckState checkState) {
        item->setCheckState(Column::EDITABLE, checkState);
    }
    void setItemReadOnly(QTreeWidgetItem *item) {
        item->setData(Column::EDITABLE, Qt::CheckStateRole, QVariant());
    }
    bool isItemSetVisible(QTreeWidgetItem *item) { return item->checkState(Column::MAP); }
    bool isItemSetEditable(QTreeWidgetItem *item) { return item->checkState(Column::EDITABLE); }
    bool isMapColumn(int col) { return col == Column::MAP; }
    bool isEditableColumn(int col) { return col == Column::EDITABLE; }

  signals:
    void requestShowLink(const QUrl &url);

  public slots:
    void removeAllItem(QTreeWidgetItem *start);
    QTreeWidgetItem *addNewItem(const QString &title, QTreeWidgetItem *parent = NULL);

  private:
    QStringList columnNames = (QStringList() << m_mapColumn << m_editableColumn);
};

QT_END_NAMESPACE

#endif // BOOKMARK_WIDGET_H
