// fieldsel.h				emacs, this is written in -*-c++-*-
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef FIELDSEL_H
#define FIELDSEL_H


#include <QBitArray>
#include <QDialog>
#include <QCheckBox>

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QCloseEvent>

#include "proc.h"


class FieldSelect : public QDialog {
    Q_OBJECT
public:
    FieldSelect(Procview *pv);

    void update_boxes();

public slots:
    void field_toggled(bool);
    void closed();

signals:
    void added_field(int);
    void removed_field(int);

protected:
    QCheckBox **buts;
    int nbuttons;
    QBitArray disp_fields;
    bool updating;
    Procview *procview;

    void set_disp_fields();
    void closeEvent(QCloseEvent *);
	virtual void showEvent( QShowEvent * );
};

#endif	// FIELDSEL_H
