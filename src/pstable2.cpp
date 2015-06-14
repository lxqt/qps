
#include "pstable2.h"
int flag_x=0;
PstableModel::PstableModel(QObject *parent,Procview *pv): HtableModel(parent)
{
	procview=pv;
}


QModelIndex PstableModel::index(int row, int col, const QModelIndex &parent) const
{
	if(flag_x)	
	printf("index(): row=%d col=%d  parent(%d %d)\n",row,col,parent.row(),parent.column());
	//		if(row<0 or column<0)  // root?
	//			return QModelIndex();
	Procinfo *pi;

	if(col <0 or col>=procview->cats.size()) return QModelIndex();
		//qFatal("Qps::Pstable2::sizeHintForColumn() Bug!: over col !! %d\n",col);
	if(parent==QModelIndex())
	{ 
		if(row>=0 and row<procview->root_procs.size())
		{
			Procinfo *pi=procview->root_procs[row];  // NULL possible!!
			return createIndex(row,col,pi);
		}
		else 
			printf("out of row %d",row);  // ???????????????????
	}

	if(parent.isValid()){

		Procinfo *ppi= static_cast<Procinfo*>(parent.internalPointer());

		if (ppi and row < ppi->table_children.size() and row>=0)
		{
			Procinfo *pi=ppi->table_children[row];
			return createIndex(row,col,pi);
		}
	}
	printf("	indeX() error\n");
	//qFatal("Error : index() row=%d ",row);
	return QModelIndex();
}

//Pure Virtual
QModelIndex PstableModel::parent(const QModelIndex &child) const
{ 
	//wrong	parent makes segfault
	//return QModelIndex();
//	printf("parent() ! %d %d\n",child.row(),child.column());
	if(child.isValid())
	{
	 	Procinfo *pi= static_cast<Procinfo*>(child.internalPointer());
		//if (pi==NULL or pi->level==0 )//or pi->parent_row<0)	
		if( pi->level<1 )//or pi->parent_row<0)	
		{
		//	printf("root , pid=%d ppid=%d \n",pi->pid,pi->ppid);
			return QModelIndex();
		}

		//parent_row == -1 segfault
	 	Procinfo *ppi=procview->procs.value(pi->ppid,0);  ///
		if(ppi==NULL) printf("error xxxxxxxxxxxx\n");
		//if(pi->ppid==2608)	printf("parent_row=%d ppid=%d\n",pi->parent_row,pi->ppid);
	 //	Procinfo *ppi=procview->linear_procs[pi->parent_row];
     	//if (pi->ppid==0 or ppi==NULL)// or !procview->treeview)
		////if(flag_x)	printf("parent : pid=%d  (row=%d ppid=%d)\n",pi->pid,ppi->table_child_seq,ppi->pid);
		return createIndex(ppi->table_child_seq , 0 ,ppi);
		//return createIndex(0 , 0 ,ppi);
	}
	printf("parent() Invalud\n");
	return QModelIndex(); // no parent!
   		
//	if(row>=0 and col>=0 and row<htable->nrows)
//		return createIndex(htable->parentRow(row),col,NULL);
}

int PstableModel::columnCount(const QModelIndex &parent) const
{
	if (parent.column() > 0)	return 0;
//	printf("columnCount() size=%d\n" ,procview->cats.size());
	return procview->cats.size(); 
}

//pure virtual: return chilren count  
int PstableModel::rowCount(const QModelIndex &parent) const
{
	if (parent.column() > 0) return 0;

	int count=0;
	if(parent==QModelIndex())
	{
		count=procview->root_procs.size();
//		printf("rowCount() root_procs size=%d\n" ,count);
	}
    else {
		Procinfo *pi= static_cast<Procinfo*>(parent.internalPointer());
		count=pi->table_children.size();
	}
//	printf("rowCount (%d %d), %d\n",parent.row(),parent.column(),count);
	//if(pi->table_children.size()>0)	htable->setExpanded(index,false);
//	if(count>0)	htable->setExpanded(parent,false);
//	if(count>0)	htable->expand(parent);
	return count;
}

bool PstableModel::hasChildren ( const QModelIndex & parent ) const
{
	int count;
	if (parent.column() > 0) return false;
	return rowCount(parent);
}

QVariant PstableModel::data(const QModelIndex &index, int role) const
{	
//	printf("data\n");	
	Procinfo *pi= static_cast<Procinfo*>(index.internalPointer());
	int col=index.column();
		
		if (role == Qt::DisplayRole)
		{
		//	if(pi->child_seq_prev>=0 and pi->child_seq!=index.row())
		//	printf("========= not equal row =====\n");
			return procview->cats[index.column()]->string(pi);
		//	returhtable->text(index.row(),index.column());
		} else
		if (role == Qt::DecorationRole) {
		} else
		if (role == Qt::EditRole) {
		} else
		if(role==Qt::CheckStateRole)
		{
		//	if(item->enable)
		//		return Qt::Checked;
		//	else 
		//		return Qt::Unchecked;
		}else
		if(role==Qt::TextAlignmentRole)
		{
			Category *cat = procview->cats[index.column()];
			return cat->alignment();
		}
		if (role == Qt::EditRole) 
		{

		}else
		if(role==Qt::BackgroundColorRole)
		{

		}else
		if (role == Qt::SizeHintRole)
	{
	//	return QSize(18,18);
	}
	return QVariant();
}
// MOVE TO Procview
// slot: called when a title is clicked

//void Pstable::set_sortcol() 
//	setSortedCol(i);
// connect(header(), SIGNAL(sectionPressed( int )),SLOT(setSortColumn(int ))); 
void Pstable2::setSortColumn(int col)
{
	// col=-1 no section
	if(col <0 or col>=procview->cats.size()) return;
		//qFatal("Qps::Pstable2::setSortColumn() Bug!: over col !! %d\n",col);
	//printf("setSortColumn()\n");
	procview->setSortColumn(col,false);
	setSortedCol(col);	//void HeadedTable::setSortedCol(int col)
	refresh(); // no redraw
	model->update();
	
}

// sync Procview and HeadedTable
// set sorted column of table to procview->sortcol
void Pstable2::set_sortcol() 
{
	for(int i = 0; i < procview->cats.size(); i++)
	{
		if(procview->cats[i] == procview->sortcat) {
			setSortedCol(i);	// HeadedTable
			return;
		}
	}
	setSortedCol(-1);
}


// for Speed Optimize
int Pstable2::sizeHintForColumn(int col) const
{
	if(col <0 or col>=procview->cats.size()) qFatal("Qps::Pstable2::sizeHintForColumn() Bug!: over col !! %d\n",col);
	//int aw=fontMetrics().averageCharWidth();
 	int aw=fontMetrics ().width("0");
	//int title_witdh=fontMetrics().width(procview->cats[col]->name);
 	//static int z5=fontMetrics ().width("00000");
 	//static int z6=fontMetrics ().width("000000");
	int cat_id=procview->cats[col]->index;
	switch(cat_id)
	{	
// 	only COMMON Field
//		case F_PID :
		case F_RSS :
		case F_NICE :
		case F_SIZE :
		case F_TIME :
			return aw*6;
		case F_TTY :
		case F_MEM :
		case F_CPU :
			//return aw*7;
			return fontMetrics().width("%MEM")+10;
		case F_WCPU :
			return fontMetrics().width("%WCPU") +10;
			//return aw*4; 
		case F_USER :
		case F_CMD :
//		case F_CMDLINE:	return 300;
		default:
			return QTreeView::sizeHintForColumn(col);
			//return HeadedTable::sizeHintForColumn(col);
			//QTreeView::resizeColumnToContents (col);
			return -1;
			;
	}
	return -1;
}

// inner 
QString Pstable2::title(int col)
{
	if(col <0 or col>=procview->cats.size())
	{
		qFatal("Qps::Pstable2::title(): Bug! over col !! %d\n",col);
		return "";
	}
	return procview->cats[col]->name;
}

QString Pstable2::text(int row, int col) //not called
{
	if(col <0 or col>=procview->cats.size()) qFatal("Qps::Pstable2::text() Bug!: over col !! %d\n",col);

	return procview->cats[col]->string(procview->linear_procs[row]);
}

int Pstable2::totalRow()//???
{
	return procview->linear_procs.size();
}

int Pstable2::colWidth(int col)
{
	if(col <0 or col>=procview->cats.size()) qFatal("Qps::Pstable2::colWidth() Bug!: over col !! %d\n",col);
///	return 30;
	// this is -1 for variable width fields, htable keeps track of it
	return procview->cats[col]->width();
}

int Pstable2::alignment(int col)
{
	if(col <0 or col>=procview->cats.size()) qFatal("Qps::Pstable2::alignment() Bug!: over col !! %d\n",col);
	//printf("debug:alignment()\n");
	Category *cat = procview->cats[col];
	return cat->alignment();
}



// virtual of HeadedTable 
void Pstable2::setSelected(int row, bool sel)
{
///	printf("debug:setSelected()\n");
	Procinfo *pi= procview->linear_procs[row];
	if(pi->selected!=sel)
	{
		pi->selected=sel; 
//		body->repaintRow(row);  // TEMP trick..
	}
}

// virtual 
bool Pstable2::isSelected(int row) 
{ 
	Procinfo *pi= procview->linear_procs[row];
	return pi->selected; 
}

int Pstable2::rowDepth(int row)
{
//	if(row < procview->linear_procs.size())
	Procinfo *pi= procview->linear_procs[row];
	if(pi)
		return pi->level;
	
	printf("Qps bug: over row  %d!!!\n",row);
	return 0;
}

bool Pstable2::lastChild(int row)
{
	Procinfo *pi= procview->linear_procs[row];
	if(pi)
		return pi->lastchild;
	printf("Qps bug: over row  %d!!!\n",row);
	return 0;
}

/*
int Pstable2::childCount(int row)
{
	return procview->linear_procs[row]->table_children.size();
	//and !p->hidekids) 
} */

// Segfault !!!!
// return  not PPID ! just virtual parent in tree
int Pstable2::parentRow(int row)
{
	return procview->linear_procs[row]->parent_row;
	/* int ppid= procview->linear_procs[row]->ppid;
	Procinfo *pi= procview->getProcinfoByPID(ppid);
	if(pi) return ppid;
	else return -1; */
}

//DRAFT
QString Pstable2::tipText(int col)
{
	Category *cat = procview->cats[col];
	QString s(cat->help);
	
	// trick
	if(cat->index == F_STAT)
		s.append("\n(R =Running, S =Sleeping, T =sTopped, Z=Zombie)");
	if(cat->index == F_PLCY)
		s.append("\n(TS =Time Sharing)");
	if(cat->index == F_RSS);
		//s.append("\nRSS = CODE + DATA + SHARE\n"
		//	   "RSS = TRS  + DRS  + SHARE\n");
		///s.append("\n(RSS = TRS + DRS)");
	if(cat == procview->sortcat)
		s.append(procview->reversed ? "\n(sorted backwards)" : "\n(sorted)");
	return s;
}

//DRAFT  who call?
char* Pstable2::total_selectedRow(int col)
{
	return 0;
}

Pstable2::Pstable2(QWidget *parent,Procview *pv)
	: HeadedTable2(parent, 0)
{
	printf("read 3-2\n");
	procview=pv;
	model=new PstableModel(this,pv);
	setModel(model);
	// activate() double click
	// pressed() one click
	// entered() drag
	//connect(this, SIGNAL(activated ( const QModelIndex & )),SLOT(selection_update(const QModelIndex&))); 
	connect(this, SIGNAL(pressed ( const QModelIndex & )),SLOT(selection_update(const QModelIndex&))); 
//	connect(header(), SIGNAL(sectionClicked( int )),SLOT(setSortColumn(int ))); 
	connect(header(), SIGNAL(sectionPressed( int )),SLOT(setSortColumn(int ))); 
//	connect(this, SIGNAL(titleClicked(int)), SLOT(setSortColumn(int)));
	
	printf("read 3-3\n");
	//modelIterate(rootIndex());
	//modelIterate(QModelIndex());
	//update(QModelIndex());
	//reset();
//	expandAll();
//	reset();
	printf("read 3-4\n");
}

// who call this ? : from qps.cpp
void Pstable2::setProcview(Procview *pv)
{
	procview = pv;
}

HeadedTable2::NodeState Pstable2::folded(int row)
{
	Procinfo *p = procview->linear_procs[row];
	return ( p->table_children.size() > 0) ? (p->hidekids ? Closed : Open) : Leaf;
}


// slot: called when selection changes
//	called by 
void Pstable2::selection_update(const QModelIndex &idx)
{
	QModelIndexList list=selectionModel()->selectedRows(); 
	printf("debug:selection_update()\n");
	//qps->update_menu_selection_status();
}

// slot: changes table mode
// call by 
// 	1.void Qps::set_table_mode(bool treemode)
void Pstable2::setTreeMode(bool treemode)
{
	/// printf("Pstable setTreeMode\n");
	procview->treeview = treemode;
	procview->fieldArrange();
	set_sortcol();
	HeadedTable2::setTreeMode(treemode);  
	Pstable2::refresh();
	if(treemode)
		setRootIsDecorated(true);
	else setRootIsDecorated(false);
	
	reset();
	expandAll();
	//model->update(); //SEGFAULT
	//rebuild();
}


bool Pstable2::columnMovable(int col)
{
	if(treemode) 
	{
		if(col==0) return false;

	}
	if(procview->cats[col]->index==F_CMDLINE)
		return false;

	return true;
}

// called by HeadedTable
// Description : FIELD movement by mouse drag 
//	 				To place From col
//	virtual HeadedTable::moveCol(col,place);
void Pstable2::moveCol(int col, int place) 
{
	//printf("Pstable::moveCol\n");
	procview->moveColumn(col,place);
	set_sortcol() ;
//	update();
	refresh(); // width size changed ,...
}


// DRAFT CODE:
// 1.procview->refresh: proc.refresh, rebuild
// 2.resetwidth
// 3.repaint

// called by 
// 	1.void Qps::refresh()
void Pstable2::refresh()
{
    printf("Pstable2:refresh()\n");
	procview->rebuild(); //for Table
	setNumCols(procview->cats.size());  		//2. resetWidths() 
	setNumRows(procview->linear_procs.size()); 	//1.
//	printf("catsize=%d row=%d\n",procview->cats.size(),procview->linear_procs.size());
//	update();
//	update(QModelIndex());
//	repaint();
//	repaint_changed();
	//model->revert();	//temp					
//	model->submit();	//temp	
//	model->update();				
}


