// indent by VIM
// fieldsel.cpp
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#include "fieldsel.h"

FieldSelect::FieldSelect(Procview *pv)
{
    nbuttons = pv->categories.size();
    disp_fields.resize(64);
    procview = pv;

    int half = (nbuttons + 1) / 2;
    updating = false;

    setWindowTitle("Select Custom Fields ");
    QBoxLayout *v_layout = new QVBoxLayout;
    setLayout(v_layout);

    QGridLayout *grid = new QGridLayout;
    grid->setSpacing(0);
    grid->setColumnMinimumWidth(2, 17);
    v_layout->addLayout(grid);

    buts = new QCheckBox *[nbuttons];

    QList<int> keys = pv->categories.keys();

    for (int i = 0; i < nbuttons; i++)
    {
        Category *cat = pv->categories[keys.takeFirst()]; // fieldlist

        QCheckBox *but = new QCheckBox(cat->name, this);
        QLabel *desc = new QLabel(cat->help, this);
        if (i < half)
        {
            grid->addWidget(but, i, 0);
            grid->addWidget(desc, i, 1);
        }
        else
        {
            grid->addWidget(but, i - half, 3);
            grid->addWidget(desc, i - half, 4);
        }
        buts[i] = but;
        connect(but, SIGNAL(toggled(bool)), this, SLOT(field_toggled(bool)));
    }
    update_boxes();

    QPushButton *closebut = new QPushButton("Close", this);
    connect(closebut, SIGNAL(clicked()), SLOT(closed()));
    closebut->setFocus();

    v_layout->addWidget(closebut);
    //	v_layout->freeze(); ///!!!
}

// CALLBACK : one of the fields was toggled (we don't know which one yet)
void FieldSelect::field_toggled(bool)
{
    if (updating)
        return;
    set_disp_fields();

    for (int i = 0; i < nbuttons; i++)
    {
        Category *cat = procview->cat_by_name(buts[i]->text().toLatin1());

        if (buts[i]->isChecked() != disp_fields.testBit(cat->id))
        {
            if (buts[i]->isChecked())
                emit added_field(cat->id); // send cat_index
            else
                emit removed_field(cat->id);
        }
    }
}

void FieldSelect::closed() { hide(); }

void FieldSelect::closeEvent(QCloseEvent *) { closed(); }

void FieldSelect::set_disp_fields()
{
    disp_fields.fill(false);
    int n = procview->cats.size();
    for (int i = 0; i < n; i++)
        disp_fields.setBit(procview->cats[i]->id);
}

void FieldSelect::showEvent(QShowEvent *) { update_boxes(); }

// init check button ?
void FieldSelect::update_boxes()
{
    set_disp_fields();

    updating = true;
    for (int i = 0; i < nbuttons; i++)
    {
        Category *cat = procview->cat_by_name(buts[i]->text().toLatin1());
        buts[i]->setChecked(disp_fields.testBit(cat->id));
    }
    updating = false;
}
