/*
 * prefs.cpp
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

#include "prefs.h"
#include "proc.h"
#include "qps.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QGroupBox>
#include <qstring.h>
#include <QtGui>
#include <QApplication>
#include <QFontComboBox>

//#include <qstylefactory.h>
#include "qps.h"
extern Qps *qps;

extern QFontComboBox *font_cb;
// class that validates input for the "swap warning limit" field
class Swapvalid : public QValidator
{
  public:
    Swapvalid(QWidget *parent) : QValidator(parent) {}
    virtual State validate(QString &s, int &) const;
};

struct Boxvar
{
    const char *text;
    bool *variable;
    // CrossBox *cb;
    QCheckBox *cb;
};

static Boxvar general_boxes[] = {
    // {"Graphic Load Display", &Qps::show_load_graph, 0},
    // {"Graphic CPU Display", &Qps::show_cpu_bar, 0},
    //{"Minimized on Close Button", &Qps::flag_systray, 0},
    {"Exit On Close Button", &Qps::flag_exit, 0},

    // TEMPO
    //  {"Use Tab-View", &Qps::flag_useTabView, 0},
    //   {"Hide qps in Linear mode", &Qps::flag_qps_hide, 0},
    //  {"Load Graph in Icon", &Qps::load_in_icon, 0},
    //  {"Selection: Copy PIDs to Clipboard", &Qps::pids_to_selection, 0},
    //   {"show underdevelopment ", &Qps::flag_devel, 0},
    //   {"Vertical CPU Bar (under development)", &Qps::vertical_cpu_bar, 0},
    {0, 0, 0}};

#ifdef LINUX
static Boxvar sockinfo_boxes[] = {
    {"Host Name Lookup", &Qps::hostname_lookup, 0},
    {"Service Name Lookup", &Qps::service_lookup, 0},
    {0, 0, 0}};
#endif

static Boxvar tree_boxes[] = {{"Disclosure Triangles", &Qps::tree_gadgets, 0},
                              {"Branch Lines", &Qps::tree_lines, 0},
                              {0, 0, 0}};

static Boxvar misc_boxes[] = {
    {"Auto Save Settings on Exit", &Qps::auto_save_options, 0},
    {"Selection: Copy PIDs to Clipboard", &Qps::pids_to_selection, 0},
#ifdef SOLARIS
    {"Normalize NICE", &Qps::normalize_nice, 0},
    {"Use pmap for Map Names", &Qps::use_pmap, 0},
#endif
    {0, 0, 0}};

struct Cbgroup
{
    const char *caption;
    Boxvar *boxvar;
};

static Cbgroup groups[] = {{"General", general_boxes},
#ifdef LINUX
//    {"Socket Info Window", sockinfo_boxes},
#endif
                           //   {"Tree View", tree_boxes},
                           //    {"Miscellaneous", misc_boxes},
                           {0, 0}};

void find_fontsets();
// dual use function: both validate and apply changes
QValidator::State Swapvalid::validate(QString &s, int &) const
{
    // only accept /^[0-9]*[%kKmM]?$/
    int len = s.length();
    int i = 0;
    while (i < len && s[i] >= '0' && s[i] <= '9')
        i++;
    if ((i < len && QString("kKmM%").contains(s[i]) == 0) || i < len - 1)
        return Invalid;
    if (s[i] == 'k')
        s[i] = 'K';
    if (s[i] == 'm')
        s[i] = 'M';
    // int val = atoi((const char *)s);
    int val = s.toInt();
    bool percent;
    if (s[i] == '%')
    {
        percent = true;
    }
    else
    {
        percent = false;
        if (s[i] == 'M')
            val <<= 10;
    }
    Qps::swaplimit = val;
    Qps::swaplim_percent = percent;
    return Acceptable;
}

Preferences::Preferences() : QDialog()
{
    int flag_test = 0;
    setWindowTitle("Preferences");
    QVBoxLayout *v_layout = new QVBoxLayout;

    if (flag_test)
    {
        QTabWidget *tbar = new QTabWidget(this);
        QWidget *w = new QWidget(this);
        tbar->addTab(w, "&Setting");
        w->setLayout(v_layout);
    }
    else
        setLayout(v_layout);

    v_layout->setSpacing(1);
    // v_layout->setSpacing(1);
    const int border_x = 10;
    int min_x = 0;

    for (Cbgroup *g = groups; g->caption; g++)
    {
        QGroupBox *grp = new QGroupBox(g->caption, this);
        QVBoxLayout *vbox = new QVBoxLayout;
        for (Boxvar *b = g->boxvar; b->text; b++)
        {
            b->cb = new QCheckBox(b->text, grp);
            vbox->addWidget(b->cb);
            connect(b->cb, SIGNAL(clicked()), SLOT(update_reality()));
            // -> EMIT prefs_change()
        }
        grp->setLayout(vbox);
        v_layout->addWidget(grp);
    }
    update_boxes();

    /*
    QGroupBox *wgrp = new QGroupBox("Swap Warning", this);
  QHBoxLayout *hl = new QHBoxLayout;
    wgrp->setLayout(hl);
    QLabel *lbl = new QLabel("Warn when free swap below", wgrp);
    QLineEdit *swaped = new QLineEdit(wgrp);
  hl->addWidget(lbl);
  hl->addWidget(swaped);
    //swaped->setAlignment (  Qt::AlignRight );
    QString s;
    if(Qps::swaplim_percent) {
            s.sprintf("%d%%", Qps::swaplimit);
    } else {
            if(Qps::swaplimit >= 1024 && (Qps::swaplimit & 0x7ff) == 0)
                    s.sprintf("%dM", Qps::swaplimit >> 10);
            else
                    s.sprintf("%dK", Qps::swaplimit);
    }
    swaped->setText(s);
    swaped->setValidator(new Swapvalid(this));
    connect(swaped, SIGNAL(textChanged(const QString &)),
  SIGNAL(prefs_change()));
    v_layout->addWidget(wgrp);
    */

    rb_totalcpu = NULL; // tmp

    if (QPS_PROCVIEW_CPU_NUM() > 1)
    {
        QGroupBox *grp_cpu = new QGroupBox("%CPU divided by", this);
        QVBoxLayout *vboxlayout = new QVBoxLayout;
        QHBoxLayout *hbox = new QHBoxLayout;
        vboxlayout->addLayout(hbox);

        // num_cpus
        QString str;
        str.sprintf("Total cpu: %d", QPS_PROCVIEW_CPU_NUM());
        rb_totalcpu = new QRadioButton(str, grp_cpu);
        QRadioButton *rb2 = new QRadioButton("Single cpu: 1", grp_cpu);
        if (!Procview::flag_pcpu_single)
            rb_totalcpu->setChecked(true);
        else
            rb2->setChecked(true);

        rb_totalcpu->setToolTip("default");
        rb2->setToolTip("for developer");
        hbox->addWidget(rb_totalcpu);
        hbox->addWidget(rb2);
        grp_cpu->setLayout(vboxlayout);
        v_layout->addWidget(grp_cpu);

        connect(rb_totalcpu, SIGNAL(clicked()), this, SLOT(update_config()));
        connect(rb2, SIGNAL(clicked()), this, SLOT(update_config()));
    }

    // Appearance ====================================
    if (font_cb == NULL)
    {
        font_cb = new QFontComboBox(this); // preload
        font_cb->setWritingSystem(QFontDatabase::Latin);
        font_cb->setCurrentFont(QApplication::font());

        // remove Some Ugly Font : hershey...
        // printf("size=%d\n",font_cb->count());
        for (int i = 0; i < font_cb->count();)
        {
            QString name = font_cb->itemText(i);
            if (name.contains("hershey", Qt::CaseInsensitive) == true)
            {
                //		printf("%s\n",qPrintable(name));
                font_cb->removeItem(i);
            }
            else
                i++;
        }
    }

    if (font_cb)
    {
        font_cb->show();

        QGroupBox *gbox = new QGroupBox("Appearance", this);
        QVBoxLayout *vbox = new QVBoxLayout;
        QHBoxLayout *hbox = new QHBoxLayout();

        psizecombo = new QComboBox(this);
        hbox->addWidget(font_cb);
        hbox->addWidget(psizecombo);
        vbox->addLayout(hbox);
        gbox->setLayout(vbox);
        v_layout->addWidget(gbox);

        // connect(font_cb, SIGNAL(activated( int)),this,
        // SLOT(font_changed(int)));
        connect(font_cb, SIGNAL(activated(int)), this, SLOT(font_changed(int)));
        connect(psizecombo, SIGNAL(activated(int)), SLOT(font_changed(int)));

        // add to font size
        init_font_size();
    }

    /*
    // Style, Themes ==================================
    QLabel *label3 = new QLabel("Themes", font_grp);
    QComboBox *theme_combo = new QComboBox(font_grp);

    QStringList styles = QStyleFactory::keys();
  styles.sort();
  theme_combo->insertStringList(styles);

    QString currentstyle;
    currentstyle = QApplication::style().className();
  */

    QPushButton *saveButton = new QPushButton("OK", this);
    // saveButton->setFocusPolicy(QWidget::NoFocus);
    saveButton->setFocus();
    saveButton->setDefault(true);
    v_layout->addWidget(saveButton);
    // v_layout->freeze();

    connect(saveButton, SIGNAL(clicked()), SLOT(closed()));
}

//
void Preferences::init_font_size()
{
    // 	QFontDatabase db;
    //	QStringList families ( WritingSystem writingSystem = Any ) const
    //	QStringList families = db.families(QFontDatabase::Latin);
    //	QStringList extra;
    psizecombo->clear();
    int i, idx = 0;
    for (i = 5; i < 24; i++)
        psizecombo->insertItem(idx++, QString::number(i));

    // find current font size
    i = 0;
    for (int psize = QApplication::font().pointSize(); i < psizecombo->count();
         ++i)
    {
        const int sz = psizecombo->itemText(i).toInt();
        if (sz == psize)
        {
            psizecombo->setCurrentIndex(i);
            break;
        }
    }
}

// slot: update check boxes to reflect current status
void Preferences::update_boxes()
{
    for (Cbgroup *g = groups; g->caption; g++)
        for (Boxvar *b = g->boxvar; b->text; b++)
            b->cb->setChecked(*b->variable);
}

// slot: update flags and repaint to reflect state of check boxes
void Preferences::update_reality()
{
    for (Cbgroup *g = groups; g->caption; g++)
        for (Boxvar *b = g->boxvar; b->text; b++)
            *b->variable = b->cb->isChecked();
    emit prefs_change();
}

void Preferences::update_config()
{
    if (rb_totalcpu and rb_totalcpu->isChecked() == true)
        Procview::flag_pcpu_single = false;
    else
        Procview::flag_pcpu_single = true;
}

void Preferences::closed()
{
    //	delete font_cb;	font_cb==NULL;
    //	font_cb->clear();
    update_config();
    hide();
    emit prefs_change();
}

// work
void Preferences::font_changed(int i)
{
    int size = psizecombo->currentText().toInt();
    QFont font = font_cb->currentFont();
    font.setPointSize(size);

    QApplication::setFont(font);
}

// DRAFT CODE:
void Preferences::fontset_changed(int i) {}
