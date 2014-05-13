// dialogs.h			emacs, this is a -*-c++-*- file
//
// This program is free software. See the file COPYING for details.
// Author: Mattias Engdegård, 1997-1999

// misc. handy dialogs for use everywhere

#ifndef DIALOGS_H
#define DIALOGS_H

#ifndef USING_PCH 
#include <QDialog>
#include <QSlider>
#include <QGroupBox>
#include <QLineEdit>
#include <QPixmap>

#include <QMessageBox> 
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QPushButton>
#endif


#include "misc.h"

class IntervalDialog : public QDialog {
	Q_OBJECT
	public:
		IntervalDialog(const char *ed_txt, bool toggle_state);

	protected slots:
		void done_dialog();
		void event_label_changed();
	public:
		QString ed_result;
		CrossBox *toggle;

	protected:
		QPushButton *ok, *cancel;
		QLabel *label;
		QLineEdit *lined;
};

class SliderDialog : public QDialog {
    Q_OBJECT
public:
    SliderDialog(int defaultval, int minval, int maxval);

    QString ed_result;

protected slots:
    void slider_change(int val);
    void done_dialog();

protected:
    QPushButton *ok, *cancel;
    QLabel *label;
    QLineEdit *lined;
    QSlider *slider;
};

class PermissionDialog : public QDialog {
    Q_OBJECT
public:
    PermissionDialog(QString msg,QString passwd);
    QLineEdit *lined;
    QLabel *label;

//protected slots:
//    void slider_change(int val);
//  void done_dialog();
/*
protected:
    QPushButton *ok, *cancel;
    QLabel *label;
    QSlider *slider; */
};


class SchedDialog : public QDialog {
    Q_OBJECT
public:
    SchedDialog(int policy, int prio);

    int out_prio;
    int out_policy;

protected slots:
    void done_dialog();
    void button_clicked(bool );

private:
    QGroupBox *bgrp;
    QRadioButton *rb_other, *rb_fifo, *rb_rr;
    QLabel *lbl;
    QLineEdit *lined;
};
#endif	// DIALOGS_H
