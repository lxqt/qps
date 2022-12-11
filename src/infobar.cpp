/*
 * infobar.cpp
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

#include <cstdio>
#include <sys/types.h>
#include <ctime>

#include "infobar.h"
#include "proc.h"
#include "qps.h"
#include "misc.h"
#include <QBitmap>

extern ControlBar *controlbar;
extern TFrame *infobox; // testing
extern Qps *qps;        // testing

int history_start_idx = 0; // testing

float cpu_total = 0;
float cpu_idle = 0;
float cpu_used = 0;

//QMenu *m_popup;
subcpuRack *cpubar = nullptr; // DEL extra CPU_window

// ============================
gwidget *x_cpu;
gwidget *x_mem;
gwidget *x_swap;
gwidget *x_utime;
gwidget *x_load_avg;

class VCursor
{
  public:
    int px;
    int py;
    int idx;
    bool enable;
};
VCursor vcursor;

// return true if the swap meter is redlined
bool Infobar::swaplim_exceeded()
{
    return false;
}

char rotate_str[] = "|/-\\|/-\\";
int rotate_idx = 0;
char rotate_char = '|';

// who call ?
// DEL
void Infobar::refresh()
{
    if (rotate_str[++rotate_idx] == 0)
        rotate_idx = 0;
    rotate_char = rotate_str[rotate_idx];

    if (Qps::show_load_graph)
        update();
}

// DEL
// public : add point,, update Pixmap(graph)
//  called by  Qps::refresh()
void Infobar::update_load()
{
    // add_history_point2(cpu_used);
    // NEED SOLUTION!!!!!!!! draw graph before exposed
    if (isVisible())
        drawGraphOnPixmap();
}

void Infobar::drawGraphOnPixmap()
{
    if (size() != pixmap.size())
    {
        pixmap = QPixmap(size());  // resize(w, h);
        pixmap.setMask(QBitmap()); // clear the mask for drawing
    }

    QPainter p(&pixmap);
    p.fillRect(rect(), QBrush(Qt::black));

    for (int i = 0; i < wlist.size(); i++)
        wlist[i]->draw(&p);

    make_graph(width(), height(), &p);
}

// if p, then show A and hide B; otherwise do the opposite
// DEL?
void Infobar::showup()
{
    // if(cpubar->isVisible())	cpubar->raise();
}

// DEL?
void Infobar::show_and_hide(bool p, QWidget *a, QWidget *b)
{
    if (!p)
    {
        QWidget *c = a;
        a = b;
        b = c;
    }
    if (!a->isVisible())
    {
        b->hide();
        a->show();
    }
}

#define TOTAL_BAR 30
QColor total_color;
QColor part1_color;
QColor part2_color;
QColor part3_color;
// DRAFT CODE : DRAFT CODE !!
// return width
int drawSPECTRUM(QPainter *p, int x, int y, const char *name, int total,
                 int part1, int part2 = 0, int part3 = 0, int h = 0)
{
    int total_width;
    // bluelay  0,180,255
    // Move to Infor::Inforbar()
    total_color.setRgb(0, 65, 55);
    part1_color.setRgb(0, 255, 210);
    part2_color.setRgb(0, 185, 150);
    part3_color.setRgb(0, 160, 120);

    int i, w;
    int tx, ty, bar_xoffset, bar_h;
    int bar;

    char buff[32] = "NO";

    if (total == 0)
    {
        name = strcat(buff, name);
    }

    if (h == 0)
        h = pf_char_height();

    w = 2 + pf_write(p, x + 2, y + 2, name) + 3;
    bar_xoffset = x + w;

    if (total == 0)
        return w; //**

    // draw total_dark_null line
    bar_h = h - 1;
    ty = y + 2;
    tx = 0;
    p->setPen(total_color);
    for (i = 0; i < TOTAL_BAR; i++)
    {
        p->drawLine(bar_xoffset + tx, ty, bar_xoffset + tx, ty + bar_h);
        tx++;
        tx++;
    }
    total_width = w + tx;

    if (total == 0)
        total = 1;
    tx = 0;
    // draw part1
    p->setPen(part1_color);
    bar = part1 * TOTAL_BAR / total;
    if (bar == 0 and part1 != 0)
        bar = 1;
    for (i = 0; i < bar; i++)
    {
        p->drawLine(bar_xoffset + tx, ty, bar_xoffset + tx, ty + bar_h);
        tx += 2;
        // tx++;
    }

    if (part2 >= 0)
    {
        p->setPen(part2_color);
        bar = part2 * TOTAL_BAR / total;
        if (bar == 0 and part2 != 0)
            bar = 1;
        for (i = 0; i < bar; i++)
        {
            p->drawLine(bar_xoffset + tx, ty, bar_xoffset + tx, ty + bar_h);
            tx += 2;
            // tx++;
        }
    }
    if (part3 >= 0)
    {
        p->setPen(part3_color);
        bar = part3 * (TOTAL_BAR) / total;
        for (i = 0; i < bar; i++)
        {
            p->drawLine(bar_xoffset + tx, ty, bar_xoffset + tx, ty + bar_h);
            tx += 2;
        }
    }
    // this occured by reporter.
    // if(tx>TOTAL_BAR*2 ) printf("Error: %s total =%d, part1=%d, part2=%d
    // part3=%d\n",name,total,part1,part2,part3);
    return total_width;
}

// CPU core graph
int drawSPECTRUM2(QPainter *p, int x, int y, char *name, int total,
                  int part1, int part2 = 0, int part3 = 0,
                  int bar_h = 9, int bar_w = TOTAL_BAR)
{
    int total_width;
    // Move to Infor::Inforbar()
    total_color.setRgb(0, 67, 75); // dark
    part1_color.setRgb(0, 255, 210);
    part2_color.setRgb(0, 220, 175);
    part3_color.setRgb(0, 180, 150);

    int i, w;
    int tx, ty, bar_xoffset;
    int bar;

    char buff[32] = "NO";

    if (total == 0)
    {
        name = strcat(buff, name);
    }

    w = 2 + pf_write(p, x + 2, y + 2, name) + 2;
    bar_xoffset = x + w;

    if (total == 0)
        return w; //**

    // draw total_dark_null line
    bar_h = bar_h - 1;

    ty = y + 2;
    tx = 0;
    // draw Total_dark_bar
    p->setPen(total_color);
    for (i = 0; i < bar_w; i++)
    {
        p->drawLine(bar_xoffset + tx, ty, bar_xoffset + tx, ty + bar_h);
        tx += 2;
    }
    total_width = w + tx;

    if (total == 0)
        total = 1; // safer divide error.

    tx = 0;
    // draw part1
    p->setPen(part1_color);
    bar = part1 * bar_w / total;
    if (bar == 0 and part1 != 0)
        bar = 1;
    for (i = 0; i < bar; i++)
    {
        p->drawLine(bar_xoffset + tx, ty, bar_xoffset + tx, ty + bar_h);
        tx += 2;
    }

    if (part2 >= 0)
    {
        p->setPen(part2_color);
        bar = part2 * bar_w / total;
        if (bar == 0 and part2 != 0)
            bar = 1;
        for (i = 0; i < bar; i++)
        {
            p->drawLine(bar_xoffset + tx, ty, bar_xoffset + tx, ty + bar_h);
            tx += 2;
        }
    }
    if (part3 >= 0)
    {
        p->setPen(part3_color);
        bar = part3 * (bar_w) / total;
        for (i = 0; i < bar; i++)
        {
            p->drawLine(bar_xoffset + tx, ty, bar_xoffset + tx, ty + bar_h);
            tx += 2;
        }
    }
    // this occured by reporter.
    if (tx > TOTAL_BAR * 2)
    {
        printf("Error: TOTAL_BAR*2=%d, name= %s total =%d, part1=%d, "
               "part2=%d "
               "part3=%d\n",
               TOTAL_BAR * 2, name, total, part1, part2, part3);
        return -1;
    }
    return total_width;
}

#define TIMEDIFF(kind)                                                         \
    procview->cpu_times(cpu_id, Proc::kind) -                                  \
        procview->old_cpu_times(cpu_id, Proc::kind)
class w_cpu : public gwidget
{
  private:
    long total, user, system, idle, nice, wait;

  public:
    void draw(QPainter *p) override
    {
        x = 0;
        char buff[32];
        int cpu_id;
        int cpu_n;
        int gheight = pf_char_height() + 1;
        int mw = 0;

        user = 0, system = 0, idle = 0, nice = 0, wait = 0;

        cpu_n = procview->num_cpus;

        width = 0;

        // show Total CPU
        cpu_id = cpu_n;
        user = TIMEDIFF(CPUTIME_USER);
#ifdef LINUX
        nice = TIMEDIFF(CPUTIME_NICE);
#endif
        system = TIMEDIFF(CPUTIME_SYSTEM);
#ifdef SOLARIS
        wait = TIMEDIFF(CPUTIME_WAIT);
#endif
        idle = TIMEDIFF(CPUTIME_IDLE);
        total = user + system + wait + nice + idle;
        width = drawSPECTRUM(p, 0, 0, "CPU", total, user, system, nice,
                                 gheight - 1);

        for (cpu_id = 0; cpu_id < cpu_n; cpu_id++)
        {
            user = TIMEDIFF(CPUTIME_USER);
            idle = TIMEDIFF(CPUTIME_IDLE);
            system = TIMEDIFF(CPUTIME_SYSTEM);
#ifdef LINUX
            nice = TIMEDIFF(CPUTIME_NICE);
#endif
#ifdef SOLARIS
            wait = TIMEDIFF(CPUTIME_WAIT);
#endif

#ifdef LINUX
            total = user + system + nice + idle;
#endif
#ifdef SOLARIS
            total = user + system + wait + idle;
#endif

            if (cpu_n > 1 && cpu_n < 129)
            {
                int bar_w = 0;
                if (cpu_n <= 2)
                    bar_w = TOTAL_BAR; // default
                else if (cpu_n < 9)
                    bar_w = TOTAL_BAR / 2;
                else // if(cpu_n<17)
                    bar_w = TOTAL_BAR / 3;

                int r, c;
                c = cpu_id / 2;
                r = cpu_id % 2;
                sprintf(buff, "%d", cpu_id);
                mw = 4 * 3 + (bar_w * 2 + 8 * 2 - 1) * c;
                if (c >= 1)
                    mw -= 6;
                if (c >= 2)
                    mw -= 6;
#ifdef LINUX
                // CPU core graphs
                (void) drawSPECTRUM2(p, mw, gheight + 1 + r * (gheight - 1),
                                     buff, total, user, system, nice,
                                     gheight - 2, bar_w);
#endif

#ifdef SOLARIS
                (void) drawSPECTRUM2(p, mw, gheight + 1 + r * (gheight - 1),
                                     buff, total, user, system, wait,
                                     gheight - 2, bar_w);
#endif
            }
        }
        if (cpu_n > 1 && cpu_n < 129)
        { // make CPU core graphs paler
            p->fillRect(0, gheight + 2, 50 + mw + 100, gheight * 4,
                        QColor(0, 0, 0, 130));
        }
        height = gheight * 1;
    }

    QString info() override
    {
        float f_user, f_nice, f_system;

        f_user = (float)user / total * 100;
        f_nice = (float)nice / total * 100;
        f_system = (float)system / total * 100;
#ifdef LINUX
        return QObject::tr("User: ") + QString::number(f_user, 'f', 1) + QObject::tr("%") + "\n"
               + QObject::tr("System: ") + QString::number(f_system, 'f', 1) + QObject::tr("%") + "\n"
               + QObject::tr("Nice: ") + QString::number(f_nice, 'f', 1) + QObject::tr("%");
#endif

#ifdef SOLARIS
        float f_wait = (float)wait / total * 100;
        return QObject::tr("User: ") + QString::number(f_user, 'f', 1) + QObject::tr("%") + "\n"
               + QObject::tr("System: ") + QString::number(f_system, 'f', 1) + QObject::tr("%") + "\n"
               + QObject::tr("Nice: ") + QString::number(f_wait, 'f', 1) + QObject::tr("%");
#endif
    };
};

class w_mem : public gwidget
{
  private:
    int used;

  public:
    void draw(QPainter *p) override
    {
        height = pf_char_height() + 4;
        x = x_cpu->xpluswidth() + 10;
#ifdef LINUX
        used = procview->mem_total - procview->mem_free -
               procview->mem_buffers - procview->mem_cached;
        width = drawSPECTRUM(p, x, 0, "MEM", procview->mem_total, used,
                             procview->mem_cached, procview->mem_buffers);
#endif

#ifdef SOLARIS
        used = procview->mem_total - procview->mem_free;
        width = drawSPECTRUM(p, x, 0, "MEM", procview->mem_total, used);
#endif
    }
    QString info() override
    {
        char str[80];

        mem_string(procview->mem_total, str);
        QString res = QObject::tr("Total: ") + QString::fromLatin1(str);

        mem_string(used, str);
        res += "\n" + QObject::tr("Used: ") + QString::fromLatin1(str);

#ifdef LINUX
        mem_string(procview->mem_cached, str);
        res +=  "\n" + QObject::tr("Cached: ") + QString::fromLatin1(str);

        mem_string(procview->mem_buffers, str);
        res += "\n" + QObject::tr("Buffer: ") + QString::fromLatin1(str);
#endif

        return res;
    };
};

class w_swap : public gwidget
{
  private:
    int used;

  public:
    void draw(QPainter *p) override
    {
        x = x_mem->xpluswidth() + 10;
        used = procview->swap_total - procview->swap_free;
        width = drawSPECTRUM(p, x, 0, "SWAP", procview->swap_total, used);
        height = pf_char_height() + 4;
    }
    QString info() override
    {
        char str[80];

        mem_string(procview->swap_total, str);
        QString res = QObject::tr("Total: ") + QString::fromLatin1(str);

        mem_string(procview->swap_free, str);
        res += "\n" + QObject::tr("Free: ") + QString::fromLatin1(str);

        mem_string(used, str);
        res += "\n" + QObject::tr("Used: ") + QString::fromLatin1(str);

        return res;
    };
};

// DRAFT CODE  !!
int drawUTIME(QPainter *p, int x, int y, long boot_time)
{
    char buff[1024];
    long u = (long)time(nullptr) - (long)boot_time;
    int up_days = u / (3600 * 24);
    u %= (3600 * 24);
    int up_hrs = u / 3600;
    u %= 3600;
    int up_mins = u / 60;
    int sec = u % 60;
    if (up_days == 0)
    {
        if (up_hrs == 0)
            sprintf(buff, "UPTIME %d:%02d", up_mins, sec);
        else
            sprintf(buff, "UPTIME %d:%02d:%02d", up_hrs, up_mins, sec);
    }
    else
        sprintf(buff, "UPTIME %dDAY%s,%d:%02d:%02d", up_days,
                (up_days == 1) ? "" : "s", up_hrs, up_mins, sec);
    return pf_write(p, x, y, buff);
}

class w_utime : public gwidget
{
  public:
    void draw(QPainter *p) override
    {
        height = pf_char_height() + 4;
        x = x_swap->xpluswidth() + 10;
        width = drawUTIME(p, x, 2, procview->boot_time);
    }
    QString info() override { return QObject::tr("Time passed after system boot"); };
};

class w_load_avg : public gwidget
{
    void draw(QPainter *p) override
    {
        height = pf_char_height() + 4;

        char buff[64];
        sprintf(buff, "1m:%1.02f, 5m:%1.02f, 15m:%1.02f", procview->loadavg[0],
                procview->loadavg[1], procview->loadavg[2]);
        width = pf_str_width(buff);
        x = parent->width() - width - 6;
        int w = x_utime->xpluswidth() + 15;
        if (x < w)
            x = w;
        pf_write(p, x, 2, buff);

        // rotating slash
        int x1 = parent->width() - pf_char_width();
        int y1 = parent->height() - pf_char_height();
        char str[2] = {0, 0};
        str[0] = rotate_char;
        pf_write(p, x1, y1, str);
    }
    QString info() override
    {
        return QObject::tr("Average CPU load each 1, 5 ,15 minutes");
    };
};

GraphBase::GraphBase(QWidget * /*parent*/, Procview *pv)
{
    procview = pv;
    npoints = 0, peak = 0, h_index = 0, dirty = true;
    official_height = 2 * pf_char_height();

    hist_size = 1280;
    history = new float[hist_size];

    setMinimumHeight(official_height);

    setMouseTracking(true);
}

IO_Graph::IO_Graph(QWidget *parent, Procview *pv) : GraphBase(parent, pv)
{
    setMinimumHeight(2 * pf_char_height() + 2);
}

void GraphBase::make_graph(int w, int h, QPainter *p)
{
    float ratio = h;

    int hsize = procview->history.size();
    int start = hsize - w;

    if (start < 0)
        start = 0;

    QPolygon pa(hsize - start); // QVector pa(npts);

    int idx = 0;
    for (int i = start; i < procview->history.size(); i++, idx++)
    {
        SysHistory *hist = procview->history[i];
        pa[idx] = QPoint(idx, h - 1 - (int)(hist->load_cpu * ratio));
    }

    if (h == official_height)
    {
        history_start_idx = start; // for MousePointer!!
        npoints = idx;
    }

    // draw scale lines
    p->setPen(QColor(0, 210, 100));
    p->drawPolyline(pa);

    dirty = false;
}

void IO_Graph::make_graph(int w, int h, QPainter *p)
{
    float ratio = 1.3; // test

    int hsize = procview->history.size();
    int start = hsize - w;

    if (start < 0)
        start = 0;

    p->setPen(QColor(80, 90, 254)); // BLUE color

    int idx = 0;
    for (int i = start; i < procview->history.size(); i++, idx++)
    {
        SysHistory *hist = procview->history[i];
        p->drawLine(idx, h - 1, idx, h - 1 - (int)(hist->load_io * ratio));
    }

    {
        history_start_idx = start; // for MousePointer!!
        npoints = idx;
    }
}

void GraphBase::drawGraphOnPixmap()
{
    if (rotate_str[++rotate_idx] == 0)
        rotate_idx = 0;
    rotate_char = rotate_str[rotate_idx];

    if (size() != pixmap.size())
    {
        pixmap = QPixmap(size());  // resize(w, h);
        pixmap.setMask(QBitmap()); // clear the mask for drawing
    }

    QPainter p(&pixmap);
    p.fillRect(rect(), QBrush(Qt::black)); // p.fill(Qt::black);

    make_graph(width(), height(), &p);
}

// DRAFT CODE  !!!
void Infobar::paintEvent(QPaintEvent *e)
{
    QRect ur = e->rect(); // update rectangle
    QPainter p(this);
    p.drawPixmap(ur, pixmap, ur);
}

void GraphBase::paintEvent(QPaintEvent *e)
{
    QRect ur = e->rect(); // update rectangle
    QPainter p(this);

    // full re-draw! by update();
    if (ur.width() == width() and ur.height() == height())
    {
        drawGraphOnPixmap();
    }

    p.drawPixmap(ur, pixmap, ur);

    return;

    // ------- Cursor testing ---------
    p.setPen(QColor(250, 159, 5));
    int rel_x = x();
    p.drawLine(vcursor.px - rel_x, 0, vcursor.px - rel_x, height());
}

// TODO: 1.sort 2. time(?)
QString doHistory(SysHistory *sysh)
{
    // first sort Procinfos by their CPU usages
    auto infos = (sysh->procs).values();
    std::sort(infos.begin(), infos.end(), [](Procinfo *a, Procinfo *b) {
        return a->pcpu > b->pcpu;
    });

    QStringList l;
    for (const auto *p : qAsConst(infos))
    {
        if (p->pcpu < 0.05) // it will be rounded to 1 decimal place
            continue;
        l << p->command + " (" + QString::number(p->pcpu, 'f', 1) + "%)";
    }
    if (l.isEmpty())
        return QString();
    l.prepend(QObject::tr("CPU Usage Summary:") + "\n");
    return l.join("\n");
}

// TODO: name change!
QString GraphBase::doHistoryTXT(SysHistory *sysh)
{
    QString str;

    char buf[128];
    sprintf(buf, "%%CPU miniHistory test");
    str += QString::fromLatin1(buf);
    for (const auto *p : qAsConst(sysh->procs))
    {
        if (p->pcpu == 0)
            continue;
        sprintf(buf, " (%.02f%%)", p->pcpu);
        str += "\n" + p->command + QString::fromLatin1(buf);
    }
    return str;
}

void GraphBase::mouseMoveEvent(QMouseEvent *e)
{
    px = e->pos().x(); // x in Infobar
    py = e->pos().y(); // y in Infobar

    // NOTE: We don't keep a real history because it would take
    // a great amount of memory over time. So, we show the last
    // history item, regardless of the cursor position.
    QString text;
    /*int idx = px + history_start_idx;
    if (idx < procview->history.size() and idx >= 0)
    {
        text += doHistoryTXT(procview->history[idx]);
    }*/
    if (!procview->history.isEmpty())
    {
        text = doHistoryTXT(procview->history.last());
    }

    infobox->setText(text);

    QPoint p = mapTo(qps, e->pos()); //??

    infobox->setPos();

    // PROBLEM : leaving old vcursor ...
    vcursor.px = p.x();
    vcursor.py = p.y();
    int rel_x = GraphBase::x();
    update(p.x() - 5 - rel_x, 0, p.x() + 5 - rel_x,
           height()); // only Vcursor update
}

// TODO: 1.sort 2. time
QString IO_Graph::doHistoryTXT(SysHistory *sysh)
{
    QString str = tr("I/O Summary:") + "\n";

    // first sort Procinfos by their commands
    auto infos = (sysh->procs).values();
    std::sort(infos.begin(), infos.end(), [](Procinfo *a, Procinfo *b) {
        return a->command < b->command;
    });

    bool noIO = true;
    char buf[73], mem_str[64];
    for (const auto *p : qAsConst(infos))
    {
        if (p->io_read_KBps == 0 && p->io_write_KBps == 0)
            continue;
        noIO = false;
        str += "\n" + p->command;
        str += " (";

        if (p->io_read_KBps > 0)
        {
            mem_string_k(p->io_read_KBps, mem_str);
            sprintf(buf, "%s/s read", mem_str);
            str += QString::fromLatin1(buf);
        }

        if (p->io_write_KBps > 0)
        {
            mem_string_k(p->io_write_KBps, mem_str);
            sprintf(buf, p->io_read_KBps > 0 ? ", %s/s write" : "%s/s write", mem_str);
            str += QString::fromLatin1(buf);
        }
        str += ")";
    }
    if (noIO)
        str += "\n" + tr("No input or output");
    return str;
}

void GraphBase::leaveEvent(QEvent *) { infobox->hide(); }

void GraphBase::mousePressEvent(QMouseEvent * /*e*/) {}

Infobar::Infobar(QWidget *parent, Procview *pv) : QFrame(parent)
{
    procview = pv;
    official_height = 3 * (pf_char_height() + 2);

    npoints = 0, peak = 0, h_index = 0, dirty = true;

    hist_size = 1280;
    history = new float[hist_size];

    setAutoFillBackground(false);
    setFrameShadow(QFrame::Sunken);
    setMinimumHeight(official_height);
    x_cpu = new w_cpu();
    x_cpu->setParent(this, pv);
    x_mem = new w_mem();
    x_mem->setParent(this, pv);
    x_swap = new w_swap();
    x_swap->setParent(this, pv);
    x_utime = new w_utime();
    x_utime->setParent(this, pv);
    x_load_avg = new w_load_avg();
    x_load_avg->setParent(this, pv);

    wlist.append(x_cpu);
    wlist.append(x_mem);
    wlist.append(x_swap);
    wlist.append(x_utime);
    wlist.append(x_load_avg);

    setMouseTracking(true);
}

Infobar::~Infobar() { delete[] history; }

// a System's Info bar
Infobar2::Infobar2(QWidget * /*parent*/, Procview *pv)
{
    official_height = 35;
    setFrameShadow(QFrame::Sunken);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QBoxLayout *layout = new QHBoxLayout();
    setLayout(layout);

    layout->setSpacing(1); // betweeen gap
    layout->setMargin(0);

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->setSpacing(1); // betweeen gap
    vlayout->setMargin(0);

    layout->addLayout(vlayout);

    basic = new Infobar(this, pv);
    vlayout->addWidget(basic);

    // IO_Graph
    io_graph = new IO_Graph(this, pv); // %IO
    vlayout->addWidget(io_graph);

    /*m_popup = new QMenu("popup", this);
    QAction *act = new QAction("Under Development", this);
    act->setDisabled(true);
    m_popup->addAction(act);*/
}

void Infobar2::updatePixmap()
{
    // NEED SOLUTION!!!!!!!! draw graph before exposed?
    if (isVisible())
    {
        basic->drawGraphOnPixmap();
        io_graph->drawGraphOnPixmap();
    }
}

// old style : relative method
// add value to the history, updating peak.
// USING
void Infobar::add_history_point(unsigned int value)
{
    // simul circular buffer
    history[h_index++] = value; // float
    if (h_index >= hist_size)
        h_index = 0;
    if (npoints < hist_size)
        npoints++;
}

// BACKUP: add value to the history, updating peak.
void Infobar::add_history_point2(float value)
{
    static float last_val = 0;

    float f;
    f = last_val - value;
    value += f / 1.5; // slow up ,slow down
    history[h_index++] = value;
    if (h_index >= hist_size)
        h_index = 0;
    if (npoints < hist_size)
        npoints++;
    if (value > peak)
        peak = value;
    else
    {
        peak = 0; // no negative values
        for (int i = 0; i < npoints; i++)
            if (history[i] > peak)
                peak = history[i];
    }

    last_val = value;
}

// return updated pixmap for use as an icon
QPixmap *Infobar::make_icon(int w, int h)
{
    if (w != icon_pm.width() or h != icon_pm.height())
    {
        icon_pm = QPixmap(w, h);
        icon_pm.setMask(QBitmap()); // remove the mask for drawing
    }
    QPainter pt(&icon_pm);
    pt.fillRect(0, 0, w, h, QBrush(Qt::black));

    make_graph(w, h, &pt, true);
    return &icon_pm;
}

void Infobar::mousePressEvent(QMouseEvent *e)
{
    emit clicked();
    if (e->button() == Qt::LeftButton)
    {
        vcursor.enable = true;
        return;
    }

    //if (e->button() == Qt::RightButton)
        //m_popup->popup(e->globalPos());
}

// only works if mouse cursor in this area
void Infobar::mouseMoveEvent(QMouseEvent *e)
{
    px = e->pos().x(); // x in Infobar
    py = e->pos().y(); // y in Infobar

    QString text;
    for (int i = 0; i < wlist.size(); i++)
    {
        if (wlist[i]->intersect(px, py))
        {
            text = wlist[i]->info();
            break;
        }
    }

    if (text.isEmpty()
        && py > pf_char_height() + 4) // below the first row
    {
        // NOTE: We don't keep a real history because it would take
        // a great amount of memory over time. So, we show the last
        // history item, regardless of the cursor position.
        /*int idx = px + history_start_idx;
        if (idx < procview->history.size() and idx >= 0)
        {
            text += doHistory(procview->history[idx]);
        }*/
        if (!procview->history.isEmpty())
        {
            text = doHistory(procview->history.last());
        }
    }

    infobox->setText(text);
    infobox->setPos();
}

void Infobar::enterEvent(QEvent *)
{
    //	infobox->hide();
}

void Infobar::leaveEvent(QEvent *)
{
    infobox->hide();
}

// DRAFT CODE
// draw the load graph on the internal pixmap, if needed
// called by
// 		1.make_icon(int w, int h)
// 		2.paintEvent()
// 		3.update_load()
void Infobar::make_graph(int w, int h, QPainter *p, bool test)
{
    float ratio = h;
    int idx = 0;

    if (h == official_height) // tmp
    {
        ratio = h - 11;
    }

    int hsize = procview->history.size();
    int start = hsize - w;

    if (start < 0)
        start = 0;
    p->setPen(QColor(0, 210, 100));

    if (test == false)
    {
        QPolygon pa(hsize - start); // QVector pa(npts);

        for (int i = start; i < procview->history.size(); i++, idx++)
        {
            SysHistory *hist = procview->history[i];
            pa[idx] = QPoint(idx, h - 1 - (int)(hist->load_cpu * ratio));
        }

        p->drawPolyline(pa);
    }
    else
    {
        int idx = 0;
        for (int i = start; i < procview->history.size(); i++, idx++)
        {
            SysHistory *hist = procview->history[i];
            p->drawLine(idx, h - 1, idx, h - 1 - (int)(hist->load_cpu * ratio));
        }
    }

    {
        history_start_idx = start; // for MousePointer!!
        npoints = idx;
    }

    dirty = false;
}

void Infobar::resizeEvent(QResizeEvent * /*e*/)
{
    drawGraphOnPixmap();
}
subcpuRack::subcpuRack(QWidget *p, Procview *pv) : QWidget(p)
{
    parent = p;
    procview = pv;
    setAttribute(Qt::WA_OpaquePaintEvent);
    QPalette pal;
    pal.setColor(QPalette::Window, QColor(0, 0, 0));
    setPalette(pal);
    setMouseTracking(true);
    setMinimumHeight(12);
}

void subcpuRack::refresh() {}

void subcpuRack::mousePressEvent(QMouseEvent */*e*/)
{
    //m_popup->popup(e->globalPos());
}

// DRAFT CODE  !!!
void subcpuRack::paintEvent(QPaintEvent * /*e*/)
{

    QPainter p(this);

    p.fillRect(rect(), QBrush(Qt::black));
    p.setPen(QColor(50, 50, 50));
    p.drawLine(0, 0, width(), 0);
}

// GraphDisplay, miniDisplay

PDisplay::PDisplay(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setMargin(0);
    vlayout->setSpacing(1);
    setLayout(vlayout);

    // ** setBackgroundColor(color); **
    QPalette pal;
    pal = palette();
    setAutoFillBackground(true);
    pal.setColor(backgroundRole(), QColor(80, 80, 80));
    setPalette(pal);
}
// a BAR in a RACK
//

Infobar2 *PDisplay::addSystem(Procview *pv)
{
    Infobar2 *bar2 = new Infobar2(this, pv);
    layout()->addWidget(bar2);

    if (false and pv->num_cpus > 9)
    {
        QWidget *rack;
        rack = new subcpuRack(this, pv);
        layout()->addWidget(rack);
    }
    return bar2; //->basic;
}
