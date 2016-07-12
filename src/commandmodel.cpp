// commandmodel.cpp
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#include "commandmodel.h"

#include "command.h"

extern QList<Command *> commands;

CommandModel::CommandModel(QObject *parent) {}
CommandModel::~CommandModel() {}

QModelIndex CommandModel::index(int row, int column,
                                const QModelIndex &parent) const
{
    if (row >= 0 and column >= 0 and row < commands.size() and column < 1)
    {
        Command *cmd = commands[row];
        return createIndex(row, column, cmd);
    }
    else
        return QModelIndex();
}
QModelIndex CommandModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}
int CommandModel::rowCount(const QModelIndex &parent) const
{
    return commands.size();
}
// int CommandModel::columnCount(const QModelIndex &parent) const{return 1;};
QVariant CommandModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        Command *cmd = static_cast<Command *>(index.internalPointer());
        return cmd->name;
    }
    if (role == Qt::DecorationRole)
    {
    }
    return QVariant();
}
void CommandModel::update() {} // TEMP
