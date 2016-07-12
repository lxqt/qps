// commandmodel.h
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#include <QAbstractItemModel>

class CommandModel : public QAbstractItemModel
{
    Q_OBJECT
  public:
    CommandModel(QObject *parent = 0);
    ~CommandModel();
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const { return 1; };
    QVariant data(const QModelIndex &index, int role) const;
    //  Qt::ItemFlags flags(const QModelIndex &index) const;
    void update(); // TEMP
};
