/*
 * screenshot.h
 * This file is part of qps -- Qt-based visual process status monitor
 *
 * Copyright 2014 dae hyun, yang <daehyun.yang@gmail.com>
 * Copyright 2015 Paulo Lieuthier <paulolieuthier@gmail.com>
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

#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QPixmap>
#include <QFrame>

#include <QCheckBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QFileDialog>

class TitleBar : public QWidget
{
    Q_OBJECT
  public:
    TitleBar(QWidget *w);

  protected:
    //    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *e);
  private slots:
  private:
    QPoint press_pos;
};

class ShotArea : public QFrame
{
    Q_OBJECT
  public:
    ShotArea();

  protected:
    //    void resizeEvent(QResizeEvent *event);

    virtual void resizeEvent(QResizeEvent *e);
    virtual void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *e);
  private slots:
  private:
    QPoint press_pos;
};

/*
class SizeGrip : public QWidget
{
    Q_OBJECT
public:
    SizeGrip();
protected:
//    void resizeEvent(QResizeEvent *event);
        void mouseMoveEvent ( QMouseEvent * event );
        void mousePressEvent(QMouseEvent *e);
private slots:
private:
        QPoint press_pos;
};
*/

class Screenshot : public QWidget
{
    Q_OBJECT

  public:
    Screenshot(QWidget *p = 0);

  protected:
    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *e);
    virtual void paintEvent(QPaintEvent *event);

  private slots:
    void newScreenshot();
    void saveScreenshot();
    void shootScreen();
    void updateCheckBox();

  private:
    void createOptionsGroupBox();
    void createButtonsLayout();
    QPushButton *createButton(const QString &text, QWidget *receiver,
                              const char *member);
    void updateScreenshotLabel();

    QPoint press_pos;
    QPixmap originalPixmap;

    QLabel *screenshotLabel;
    QGroupBox *optionsGroupBox;
    QSpinBox *delaySpinBox;
    QLabel *delaySpinBoxLabel;
    QCheckBox *hideThisWindowCheckBox;
    QPushButton *newScreenshotButton;
    QPushButton *saveScreenshotButton;
    QPushButton *quitScreenshotButton;

    QVBoxLayout *mainLayout;
    QGridLayout *optionsGroupBoxLayout;
    QHBoxLayout *buttonsLayout;
    ShotArea *shotarea;
};

#endif
