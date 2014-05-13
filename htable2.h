
#include <QTreeView>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QHeaderView>

#include <QTreeWidget>


class HeadedTable2;
class CellAttribute2
{
	public:
	QString text;
	bool selected;
	bool sorted;
	
	QColor	backColor; //table share
	QColor  foreColor; //rows share
	int xpos; 	// cols share
	int ypos; 	// rows share
	int	w;		// cols share
	int depth;	// rows share
	int	folded; // 
};



class HtableModel : public QAbstractItemModel
//class HtableModel : public QAbstractTableModel 
{
    Q_OBJECT
public:
    HtableModel(QObject *parent = 0);
    ~HtableModel(){};
    virtual QModelIndex index(int row, int column, const QModelIndex &parent=QModelIndex())const; //pure
    virtual QModelIndex parent(const QModelIndex &child) const; //pure virtual
    virtual int rowCount(const QModelIndex &parent) const; 
    virtual QVariant data(const QModelIndex &index, int role) const;
    int columnCount(const QModelIndex &parent) const;
	QVariant headerData ( int section, Qt::Orientation o, int role ) const;
	virtual Qt::ItemFlags flags ( const QModelIndex & index ) const;	
//  Qt::ItemFlags flags(const QModelIndex &index) const;
//	QMap<int, QVariant> itemData ( const QModelIndex & index ) const ;
//	void update(){ reset();}
	void update(){ emit layoutChanged();}
//	void update(const QModelIndex &idx);
//	void update(int row);
//	void update(); //TEMP
	HeadedTable2 *htable;
};



class HtableHeader : public QHeaderView 
// QHeaderView ( Qt::Orientation orientation, QWidget * parent = 0 )
{
    Q_OBJECT
public:
    HtableHeader(QWidget *parent = 0);
    ~HtableHeader(){};
	protected:
		virtual void mousePressEvent(QMouseEvent *e);
	signals:
		void rightClicked(QPoint where, int col);
		void toolTip(QPoint where, int col);
	private:	
	HeadedTable2 *htable;
};




class HeadedTable2 : public QTreeView
//class HeadedTable2 : public QTreeWidget
{
	Q_OBJECT
	public:
		HeadedTable2(QWidget *parent, int opts=0);
		~HeadedTable2(){};

		friend class HtableModel;
		HtableModel *model;

		enum NodeState { Leaf, Open, Closed };
		void repaint_changed();
		int  clickedColumn(){return 0;}; // need imp
		int sortedCol() { return sorted_col; } //ok
        
/*
		void deleteCol(int col, bool update = TRUE);
		void topAndRepaint();
		int lastRowVisible() { return body->lastRowVisible(); };
		void updateCell(int row, int col, bool erase=false);
		void updateHeading(int col) {  };
		void selectionNotify();
		void repaintColumns(int col0, int col1 = -1);
		bool treeMode() { return treemode; };
		int	tableWidth() const { return body->totalWidth()+ body->verticalScrollBar()->width(); }
        bool isCellContentsChanged(int row,int col,bool head);
		

		void selectOnly(int row);
		void clearAllSelections();
		*/

		int numRows() { return nrows; }
		int numCols() { return ncols; }
		int numSelected() { return 0; }
		void setTreeMode(bool mode);
		void setSortedCol(int col);
		void setNumRows(int rows);
		void setNumCols(int cols);
		virtual bool isSelected(int row) { return false; } 
		virtual void setSelected(int row, bool sel){};
		void modelIterate(const QModelIndex &idx);	
	signals:
		void titleClicked(int col);
		void doubleClicked(int row);
		void rightClicked(QPoint where);
		void foldSubTree(int row);
		void colMoved(int col, int place);
		
	public slots:
		void selectAll();
		void repaintAll();

	protected:

		virtual void mousePressEvent(QMouseEvent *e);
		virtual int rowDepth(int row){ return 0;} ;
		virtual NodeState folded(int row){ return Leaf;};
		virtual bool folded(QModelIndex &idx){ return false;};
		virtual int parentRow(int row){return 0;};
		virtual bool lastChild(int row){return false;};
		virtual bool columnMovable(int col){return true;};
		virtual int colWidth(int col) = 0;	//head_width
		virtual void moveCol(int col, int place){};
		virtual QString title(int col) = 0;
		virtual QString text(int row, int col) = 0;

		virtual QString tipText(int col){ return "";};
		virtual int alignment(int col){ return 0;};

	/*
		virtual char *total_selectedRow(int col);
		virtual int sizeHintForColumn(int col) const;
		virtual void hideEvent ( QHideEvent * event );
		virtual void showEvent ( QShowEvent * event );
		virtual void resizeEvent(QResizeEvent *);
	*/
		// colWidth returns width in digit units; negative means variable width.

//		virtual void drawCellContents(int row, int col, int w, int h,  QPainter *painter);
		//virtual bool modified(int row){return true;};
		bool treemode;
	private:
		// to make signals originate from the right object
/*		void emit_click_signal(int col);
		void emit_double_click_signal(int row);
		void emit_right_click_signal(QPoint where);
		void emit_fold(int row);

		inline int computedWidth(int col);
		int colOffset(int col);
		inline int colXPos(int col);
		void updateCols(int deltacols, int place, bool update);
*/
		int sorted_col;		    // column to emphasize
		int reversed_sort;		// true if sorting backwards
		int options;
		
		int nrows;
		int ncols;	

		// text cache is in logical columns (reorder causes no flush)
		QHash<int,CellAttribute2 *>	cached_attr; // indexed by (row << 16) + log_col
		QHash<int,CellAttribute2 * > cached_attr_h; // indexed by (row << 16) + log_col

};
