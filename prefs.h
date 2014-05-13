// prefs.h				emacs, this is written in -*-c++-*-
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

#ifndef PREFS_H
#define PREFS_H

#include <qdialog.h>
#include <qradiobutton.h>
#include <qlabel.h>
//#include <q3buttongroup.h>
#include <QtGui>
#include "misc.h"

class Preferences : public QDialog {
    Q_OBJECT
public:
    	Preferences();
	QComboBox *psizecombo;
//    QFontComboBox *font_cb;
	QRadioButton *rb_totalcpu;
	void init_font_size();


public slots:
    void update_boxes();
    void update_reality();
	void update_config();
    void closed();
	void font_changed(int);
	void fontset_changed(int);
signals:
    	void prefs_change();
};

#endif	// PREFS_H
