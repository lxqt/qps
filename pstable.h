// pstable.h				emacs, this is written in -*-c++-*-
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef PSTABLE_H
#define PSTABLE_H

#include "proc.h"
#include "htable.h"

class Pstable : public HeadedTable
{
    Q_OBJECT
public:
    Pstable(QWidget *parent,Procview *pv);

    void set_sortcol();
    void setProcview(Procview *pv);
    virtual void moveCol(int col, int place);
    void refresh();

	// called by super
	bool hasSelection(){return  0;};
	
	virtual bool isSelected(int row); 
	virtual void setSelected(int row, bool sel);
	virtual int totalRow();
	virtual void checkTableModel ();

		
public slots:
    // void selection_update(const Svec<int> *row);
	void setSortColumn(int col);
    void subtree_folded(int row);
	void showTip(QPoint p,int index);
    void setTreeMode(bool treemode);
	void mouseOnCell(int row,int col);
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
	virtual char* total_selectedRow(int col);
	virtual int sizeHintForColumn(int col) const;
	virtual bool columnMovable(int col);
	
	virtual void overpaintCell(QPainter *p, int row, int col, int xpos);
//	virtual bool hasChildren(int row);

	virtual void leaveEvent ( QEvent * ) ;

private:
    Procview *procview;
};

 
#endif // PSTABLE_H

