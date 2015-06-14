/**********************************************************************
** $Id: qttableview.h,v 1.46 2011/08/27 00:13:41 fasthyun Exp $
**
** Definition of QtTableView class
**
** Created : 941115
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file contains a class moved out of the Qt GUI Toolkit API. It
** may be used, distributed and modified without limitation.
**
**********************************************************************/

#ifndef QTTABLEVIEW_H
#define QTTABLEVIEW_H

#ifndef USING_PCH
#include <QScrollBar>
#include <QAbstractScrollArea>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#endif

class QScrollBar;
class QtTableView : public QAbstractScrollArea
{
    Q_OBJECT
  public:
    QWidget *view;
    int flag_view;
    int cellWidth() const;
    int cellHeight() const;
    void repaintRow(int row); // paintRow();
    void coverCornerSquare(bool);
    void clearCache() {}
    void repaintChanged();
    virtual bool isCellChanged(int r, int c) { return true; };
    virtual void eraseRight(QPainter *, QRect &r) { return; }
    virtual void checkProfile(){};
    QSize tmp_size; // testing.
    int tmp_x;
    bool test;
    QColor backColor;

  protected:
    QtTableView(QWidget *parent = 0, const char *name = 0);
    ~QtTableView();

    int numRows() const;
    int numCols() const;
    void setNumRows(int);
    void setNumCols(int);

    int topCell() const;
    int leftCell() const;
    void setTopCell(int row);
    void setLeftCell(int col);
    void setTopLeftCell(int row, int col);

    int xOffset() const;
    int yOffset() const;
    virtual void setXOffset(int);
    virtual void setYOffset(int);
    virtual void setOffset(int x, int y, bool updateScrBars = true);
    virtual void scrollTrigger(int x, int y){}; // tmp

    virtual int cellWidth(int col);
    int cellHeight(int row);
    virtual void setCellWidth(int);
    virtual void setCellHeight(int);

    int totalWidth();
    int totalHeight();

    uint tableFlags() const;
    //    bool	testTableFlags( uint f ) const;
    virtual void setTableFlags(uint f);
    void clearTableFlags(uint f = ~0);

    void setAutoUpdate(bool);

    void repaintCell(int row, int column, bool usecache = false);

    QRect cellUpdateRect() const;
    QRect viewRect() const;

    int lastRowVisible() const;
    int lastColVisible() const;

    bool rowIsVisible(int row) const;
    bool colIsVisible(int col) const;

    QScrollBar *verticalScrollBar() const;
    QScrollBar *horizontalScrollBar() const;

  private slots:
    void horSbValue(int);
    void horSbSliding(int);
    void horSbSlidingDone();
    void verSbValue(int);
    void verSbSliding(int);
    void verSbSlidingDone();

  protected:
    virtual void paintCell(QPainter *, int row, int col, bool update) = 0;
    virtual void paintEvent(QPaintEvent *);
    virtual void resizeEvent(QResizeEvent *);
    virtual void scrollContentsBy(int dx, int dy);

    int findRow(int yPos) const;
    int findCol(int xPos) const;
    int findColNoMinus(int xPos) const;

    bool rowYPos(int row, int *yPos) const;
    bool colXPos(int col, int *xPos) const;

    int maxXOffset();
    int maxYOffset();
    int maxColOffset();
    int maxRowOffset();

    int minViewX() const;
    int minViewY() const;
    int maxViewX() const;
    int maxViewY() const;
    int viewWidth() const;
    int viewHeight() const;

    void updateScrollBars();
    void updateTableSize();

    QRect cellUpdateR;

  private:
    int findRawRow(int yPos, int *cellMaxY, int *cellMinY = 0,
                   bool goOutsideView = false) const;
    int findRawCol(int xPos, int *cellMaxX, int *cellMinX = 0,
                   bool goOutsideView = false) const;
    int maxColsVisible() const;

    void updateScrollBars(uint);
    void showOrHideScrollBars();

    int nRows;
    int nCols;
    int xOffs, yOffs;
    int xCellOffs, yCellOffs;
    short xCellDelta, yCellDelta;
    short cellH, cellW; //

    uint eraseInPaint : 1;
    uint verSliding : 1;
    uint verSnappingOff : 1;
    uint horSliding : 1;
    uint horSnappingOff : 1;
    uint coveringCornerSquare : 1;
    uint sbDirty : 8;
    uint inSbUpdate : 1;

    bool enablePaint;

    uint tFlags;

    QScrollBar *vScrollBar;
    QScrollBar *hScrollBar;
};

const uint Tbl_vScrollBar = 0x00000001;
const uint Tbl_hScrollBar = 0x00000002;
const uint Tbl_autoVScrollBar = 0x00000004;
const uint Tbl_autoHScrollBar = 0x00000008;
const uint Tbl_autoScrollBars = 0x0000000C;

const uint Tbl_clipCellPainting = 0x00000100;
const uint Tbl_cutCellsV = 0x00000200;
const uint Tbl_cutCellsH = 0x00000400;
const uint Tbl_cutCells = 0x00000600;

const uint Tbl_scrollLastHCell = 0x00000800;
const uint Tbl_scrollLastVCell = 0x00001000;
const uint Tbl_scrollLastCell = 0x00001800;

const uint Tbl_smoothHScrolling = 0x00002000;
const uint Tbl_smoothVScrolling = 0x00004000;
const uint Tbl_smoothScrolling = 0x00006000;

const uint Tbl_snapToHGrid = 0x00008000;
const uint Tbl_snapToVGrid = 0x00010000;
const uint Tbl_snapToGrid = 0x00018000;

inline int QtTableView::numRows() const { return nRows; }

inline int QtTableView::numCols() const { return nCols; }

inline int QtTableView::topCell() const { return yCellOffs; }

inline int QtTableView::leftCell() const { return xCellOffs; }

inline int QtTableView::xOffset() const { return xOffs; }

inline int QtTableView::yOffset() const { return yOffs; }

inline int QtTableView::cellHeight() const { return cellH; }

inline int QtTableView::cellWidth() const { return cellW; }

inline uint QtTableView::tableFlags() const { return tFlags; }

#define testTableFlags(f) ((tFlags & f) != 0)
// inline bool QtTableView::testTableFlags( uint f ) const{ return (tFlags & f)
// != 0; }

inline QRect QtTableView::cellUpdateRect() const { return cellUpdateR; }

inline void QtTableView::updateScrollBars() { updateScrollBars(0); }

#endif // QTTABLEVIEW_H
