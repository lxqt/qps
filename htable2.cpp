
#include "htable2.h"
HtableModel::HtableModel(QObject *parent): QAbstractItemModel(parent)
{
	htable=(HeadedTable2*)parent;
}

QModelIndex HtableModel::index(int row, int column, const QModelIndex &parent) const
{
	//printf("index %d %d\n",row,column);	
	if(row>=0 and column>=0 and row<htable->nrows)
	{
		//if( column <2)
		{
		//	watchCond *item=watchlist[row];
			return createIndex(row,column,NULL);
		}
	}
	return QModelIndex();
}

//Pure Virtual
QModelIndex HtableModel::parent(const QModelIndex &child) const
{
	int row,col;
	row=child.row();
	col=child.column();
	if(row>=0 and col>=0 and row<htable->nrows)
		return createIndex(htable->parentRow(row),col,NULL);
	return QModelIndex(); // no parent!
}

//return chilren count  
int HtableModel::rowCount(const QModelIndex &parent) const
{
	return htable->nrows;
}
// why segfault?
int HtableModel::columnCount(const QModelIndex &parent) const
{
	return htable->ncols;
};

QVariant HtableModel::headerData ( int section, Qt::Orientation orientation, int role ) const
{
	
//	printf("headerData\n");	
	if (role == Qt::DisplayRole)
	{	
		return htable->title(section);
		
	}else

	if(role==Qt::TextAlignmentRole)
			return htable->alignment(section);
			//return Qt::AlignCenter;
	else
	if(role==Qt::ToolTipRole)
			return htable->tipText(section);
	else
	/*
	if (role == Qt::FontRole)
	{
		QFont f=QFont();
		f.setBold(false);
		return f;
	} */
	if (role == Qt::SizeHintRole)
	{
	//	return QSize(18,18);
	}
	return QVariant();
}

QVariant HtableModel::data(const QModelIndex &index, int role) const
{	
//	printf("data\n");	
//	watchCond *item= static_cast<watchCond*>(index.internalPointer());
		if (role == Qt::DisplayRole)
		{
			return htable->text(index.row(),index.column());
		}else
		if (role == Qt::DecorationRole) {
		}
		if (role == Qt::EditRole) {
		}  
		if(role==Qt::CheckStateRole)
		{
		//	if(item->enable)
		//		return Qt::Checked;
		//	else 
		//		return Qt::Unchecked;
		}
		if(role==Qt::TextAlignmentRole)
			return Qt::AlignRight;
		if (role == Qt::EditRole) 
		{

		}
	if (role == Qt::SizeHintRole)
	{
	//	return QSize(18,18);
	}
	return QVariant();
}

Qt::ItemFlags HtableModel::flags ( const QModelIndex & index ) const
{
	return  Qt::ItemIsEnabled|Qt::ItemIsSelectable;
}	

// HtableHeader ////////////////////////// 

HtableHeader::HtableHeader(QWidget *parent):QHeaderView(Qt::Horizontal,parent)
{
	htable=(HeadedTable2*)parent;
	setMovable (true);
}

void HtableHeader::mousePressEvent(QMouseEvent *e)
{
	QHeaderView::mousePressEvent(e);
	int col=logicalIndexAt(e->pos());
	//printf("col=%d\n",col);
	if(e->button() == Qt::RightButton)
	{
		// printf("pressed col=%d\n",col); //ok
		emit rightClicked(e->globalPos(),col);
		return;
	}
}


HeadedTable2::HeadedTable2(QWidget *parent, int opts)
	:QTreeView(parent)
//	:QTreeWidget(parent)
{
	nrows = ncols = 0;
	model=new HtableModel(this);
	HtableHeader *header=new HtableHeader(this);
	setHeader(header);	
	setModel(model);
//	setEditTriggers (QAbstractItemView::SelectedClicked );
//	tableView->setItemDelegateForColumn(1,&delegate);
 //	tableView->setItemDelegate(&delegate);
	setSelectionBehavior (QAbstractItemView::SelectRows );
	setSelectionMode (QAbstractItemView::SingleSelection);
//	setSelectionMode (QAbstractItemView::ExtendedSelection);
	//setSelectionMode (QAbstractItemView::MultiSelection);

	setUniformRowHeights (true);
	setAllColumnsShowFocus (true);	
	setFrameShape ( QFrame::NoFrame );
	setAlternatingRowColors (true);

//	v->setResizeMode (0,QHeaderView::Stretch);
	//QHeaderView *h=header ();
	header->setResizeMode (QHeaderView::ResizeToContents); // use sizeHintForColumn()
	header->setClickable (true);
//	header->setVisible(false);

}

void HeadedTable2::setTreeMode(bool mode)
{

}

void HeadedTable2::setSortedCol(int c)
{

}

void HeadedTable2::setNumCols(int c)
{
	ncols=c;
}
void HeadedTable2::setNumRows(int row)
{
	nrows=row;
}
void HeadedTable2::selectAll()
{}
void HeadedTable2::repaintAll()
{}
void HeadedTable2::repaint_changed()
{
}

extern int flag_x;
void HeadedTable2::mousePressEvent(QMouseEvent *e)
{
	printf("mousePressEvent() \n");
	QTreeView::mousePressEvent(e);
	//falg_x=1;	
	flag_x=!flag_x;
	if(e->button() == Qt::RightButton)
	{
		emit rightClicked(e->globalPos());
		return;
	}
}

// TEST & DRAFT: loop ..
void HeadedTable2::modelIterate(const QModelIndex &parent)
{
	int rows=model->rowCount(parent);
	//printf("rows=%d\n",rows);
	//expand(parent);
	setExpanded(parent,true); //				expand(ci);
	for(int i=0;i<rows;i++)
	{
		QModelIndex ci=model->index(i,0,parent);	
   		//Procinfo *pi= static_cast<Procinfo*>(ci.internalPointer());
		//if(!pi->hidekids)//	setExpanded(ci,true); //				expand(ci);
		//if(!pi->hidekids and !isExpanded(ci)) 	expand(ci);
		//if(!folded(ci) and !isExpanded(ci)) 	expand(ci);
		//if(!folded(ci) and !isExpanded(ci)) 	
		
		if(model->rowCount(ci)>0)
				modelIterate(ci);
	}
}



