// listmodel.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef LISTMODEL_H
#define LISTMODEL_H

#include <QStandardItemModel>
// class ListModel : public QAbstractItemModel
class ListModel : public QAbstractTableModel
// class ListModel : public QStandardItemModel
{
    Q_OBJECT
  public:
    ListModel(QObject *parent = 0){};
    ~ListModel(){};
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const; // pure
    QModelIndex parent(const QModelIndex &child) const; // pure virtual
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const { return 2; };
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation o, int role) const;
    //	QMap<int, QVariant> itemData ( const QModelIndex & index ) const
    //;
    void update()
    { // reset();
    }
    void update(const QModelIndex &idx);
    void update(int row);
    //  Qt::ItemFlags flags(const QModelIndex &index) const;
    //	void update(); //TEMP
};

#endif // LISTMODEL_H
