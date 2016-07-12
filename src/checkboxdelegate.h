// checkboxdelegate.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999


#ifndef CHECKBOXDELEGATE_H
#define CHECKBOXDELEGATE_H

#include <QItemDelegate>
class checkBoxDelegate : public QItemDelegate
// class checkBoxDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
  public:
    checkBoxDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
};

#endif // CHECKBOXDELEGATE_H
