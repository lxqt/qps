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
