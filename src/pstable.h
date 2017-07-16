/*
 * pstable.h
 * This file is part of qps -- Qt-based visual process status monitor
 *
 * Copyright 1997-1999 Mattias Engdegård
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#ifndef PSTABLE_H
#define PSTABLE_H

#include "proc.h"
#include "htable.h"

class Pstable : public HeadedTable
{
    Q_OBJECT
  public:
    Pstable(QWidget *parent, Procview *pv);

    void set_sortcol();
    void setProcview(Procview *pv);
    virtual void moveCol(int col, int place);
    void refresh();

    // called by super
    bool hasSelection() { return 0; };

    virtual bool isSelected(int row);
    virtual void setSelected(int row, bool sel);
    virtual int totalRow();
    virtual void checkTableModel();

  public slots:
    // void selection_update(const Svec<int> *row);
    void setSortColumn(int col);
    void subtree_folded(int row);
    void showTip(QPoint p, int index);
    void setTreeMode(bool treemode);
    void mouseOnCell(int row, int col);
    void mouseOutOfCell();

  protected:
    // implementation of the interface to HeadedTable
    virtual QString title(int col);
    virtual QString text(int row, int col);
    virtual int colWidth(int col);
    virtual int alignment(int col);
    virtual QString tipText(int col);
    virtual int rowDepth(int row);
    virtual NodeState folded(int row);
    virtual int parentRow(int row);
    virtual bool lastChild(int row);
    virtual char *total_selectedRow(int col);
    virtual int sizeHintForColumn(int col) const;
    virtual bool columnMovable(int col);

    virtual void overpaintCell(QPainter *p, int row, int col, int xpos);
    //	virtual bool hasChildren(int row);

    virtual void leaveEvent(QEvent *);

  private:
    Procview *procview;
};

#endif // PSTABLE_H
