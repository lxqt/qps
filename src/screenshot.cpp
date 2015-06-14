#include <QtGui>
#include <QSizeGrip>
#include "screenshot.h"
#include <QStatusBar>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStyle>
#include <QStyleOptionFrame>
#include <QApplication>
#include <QDesktopWidget>

/*
SizeGrip::SizeGrip()
{

        setWindowFlags( Qt::FramelessWindowHint);
} */

QSizeGrip *sg;
int window_width = 320;
int window_height = 240;

TitleBar::TitleBar(QWidget *w) : QWidget(w)
{
    //	setWindowFlags( Qt::FramelessWindowHint);
    //    resize(window_width,32);
    setMinimumHeight(32);
}

void TitleBar::mousePressEvent(QMouseEvent *)
{
    //	press_pos = e->pos();
    //	printf("x=%d y=%d\n",press_pos.x(),press_pos.y());
}

void TitleBar::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) //  Button state )
    {
        // move(e->globalX()-e->x(), e->globalY()-e->y());//
        // coe->x(),e->y());
        //		move(e->globalX() - press_pos.x(), e->globalY()-
        // press_pos.y());//
        // coe->x(),e->y());
        //		sg->move(x()-100,y()-100);
    }
}

ShotArea::ShotArea()
{
    setLineWidth(3);
    setFrameStyle(QFrame::Panel | QFrame::Raised);
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void ShotArea::resizeEvent(QResizeEvent * /* event */)
{
    // printf("ShotArea::resizeEvent()\n");
    // QPixmap pixmap = QPixmap(screenshotLabel->size());
}

void ShotArea::paintEvent(QPaintEvent *event)
{
    //	printf("ShotArea::paintEvent\n");
    //	QPainter painter(this);
    //	painter.setOpacity(1.0);
    //	painter.fillRect(10, 10, 100, 80, QColor(255,255,255,120));
}

void ShotArea::mousePressEvent(QMouseEvent *e)
{
    press_pos = e->pos();
    ///	printf("x=%d y=%d\n",press_pos.x(),press_pos.y());
}

void ShotArea::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) //  Button state )
    {
        // move(e->globalX()-e->x(), e->globalY()-e->y());//
        // coe->x(),e->y());
        //		move(e->globalX() - press_pos.x(), e->globalY()-
        // press_pos.y());//
        // coe->x(),e->y());
        //		sg->move(x()-100,y()-100);
    }
}

#include <QX11Info>
Screenshot::Screenshot(QWidget *p) : QWidget(p)
{
    // Qt::WindowTitleHint
    // Qt::CustomizeWindowHint );
    //  setWindowTitle(tr("Screenshot"));
    //	TitleBar *titlebar=new TitleBar(this);
    QStatusBar *statusbar = new QStatusBar(this);
    shotarea = new ShotArea();
    //	setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::FramelessWindowHint);
    // setWindowFlags(Qt::X11BypassWindowManagerHint);
    setStyleSheet( // padding: 2px
        "QWidget { background-color : rgba(20,20,20,70%); color: "
        "rgb(0,255,150);}"
        "QPushButton,QToolButton,QLineEdit { border-width: 1px; "
        "padding:3px; "
        "border-style: solid;  border-color: rgb(210,50,130); "
        "border-radius: 5px "
        ";}"
        "QPushButton:hover,QToolButton:hover { color:rgb(40,255,190); "
        "padding:3px; border-width: 1px; border-style: solid;  "
        "border-color: "
        "rgb(230,80,170); border-radius: 5px ;}"
        "QPushButton:pressed,QToolButton:pressed { color:rgb(0,255,150); "
        "padding:3px; border-width: 2px; border-style: solid;  "
        "border-color: "
        "rgb(210,50,130); border-radius: 5px ;}"
        "QComboBox { border-width: 1px; padding:3px; border-style: solid;  "
        "border-color: rgb(50,200,130); border-radius: 5px ;}"
        "QLabel { border-width: 1px; padding:3px; border-style: solid;  "
        "border-color: rgb(65,16,40); border-radius: 5px ;}"
        "QComboBox:drop-down { border-width: 2px; padding:0px; "
        "border-style: "
        "solid;  border-color: rgb(50,200,130); border-radius: 3px ;}");

// sg=new QSizeGrip(NULL);
//	sg->show();
//	sg->move(100,100);
// QStyle::PE_Frame
// createOptionsGroupBox();
// createButtonsLayout();

#if QT_VERSION > 0x040500

#endif

    mainLayout = new QVBoxLayout;
#if QT_VERSION < 0x040300
    mainLayout->setMargin(3); // qt-4.2
#else
    mainLayout->setContentsMargins(3, 3, 3, 3); // qt-4.3
#endif
    mainLayout->setSpacing(0);

    //	mainLayout->addWidget(titlebar);
    mainLayout->addWidget(shotarea);
    // mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(statusbar);

    //  saveScreenshotButton = createButton(tr("Save"),this,
    //  SLOT(saveScreenshot()));
    saveScreenshotButton = new QPushButton("Save");
    connect(saveScreenshotButton, SIGNAL(clicked()), this,
            SLOT(saveScreenshot()));

    statusbar->addWidget(saveScreenshotButton);
    //	statusbar->addWidget( quitScreenshotButton);
    statusbar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    setLayout(mainLayout);
    // delaySpinBox->setValue(5);

    setMouseTracking(true);

    // 	if (QApplication::desktop()->winId() < 0)
    // 		printf("Qps: Null Desktop\n");
    resize(300, 240);
}

static QPoint init_pos;
void Screenshot::mousePressEvent(QMouseEvent *e)
{

    press_pos = e->globalPos();
    init_pos = pos();
    // printf("x=%d y=%d\n",press_pos.x(),press_pos.y());
}

void Screenshot::mouseMoveEvent(QMouseEvent *e)
{
    if (e->buttons() == Qt::LeftButton) //  Button state )
    {
        move(init_pos + e->globalPos() - press_pos);
        // move(x() + e->globalX() - press_pos.x(), y()+e->globalY()-
        // press_pos.y());// coe->x(),e->y());
        // sg->move(x()-100,y()-100);
    }
}

extern bool flag_xcompmgr;
void Screenshot::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    // printf("Screenshot::paintEvent\n");
    //	setAttribute(Qt::WA_NoSystemBackground);
    if (0) // if(flag_xcompmgr)//setWindowOpacity (0.9);
           // //_NET_WM_WINDOW_OPACITY
    {
        setAttribute(Qt::WA_OpaquePaintEvent);
        //	setAttribute(Qt::WA_);
        //	p.setOpacity(0.1);
        p.setCompositionMode(QPainter::CompositionMode_Source); // need!
        p.fillRect(rect(), Qt::transparent);                    // clear!
        // 	QRect rectR(shotarea->x(),shotarea->y(),
        // shotarea->width(),shotarea->height());
        //	p.fillRect(rectR,QColor(0,255,0,10));
        //	p.fillRect(rect(),QColor(255,255,255,255));
    }

    return;
    QStyleOptionFrame opt;
    QRect rectR(shotarea->x() - 1, shotarea->y() - 1, shotarea->width() + 2,
                shotarea->height() + 2);
    opt.rect = rectR;
    opt.lineWidth = 2;
    opt.midLineWidth = 1;
    opt.state = opt.state | QStyle::State_Sunken;
    /// style()->drawControl(QStyle::CE_Header, &opt, p, this);
    style()->drawPrimitive(QStyle::PE_Frame, &opt, &p, 0);
}

void Screenshot::resizeEvent(QResizeEvent *e /* event */)
{

    //	printf("Screenshot::resizeEvent()\n");
    //	QWidget::resizeEvent(e);
    //	return;
    if (flag_xcompmgr == false)
    {
        QPixmap pixmap = QPixmap(size());
        QPainter painter(&pixmap);
        // painter.eraseRect(0,0,width(),height());
        painter.fillRect(0, 0, width(), height(), QColor(0, 0, 0));
        painter.fillRect(shotarea->x() + 1, shotarea->y() + 1,
                         shotarea->width() - 2, shotarea->height() - 2
                         // painter.fillRect(shotarea->geometry()
                         ,
                         QColor(255, 255, 255));
#if QT_VERSION >= 0x040300
        setMask(
            pixmap.createMaskFromColor(QColor(255, 255, 255), Qt::MaskInColor));
// setMask(pixmap.mask()); // with alpha channel ,notwork
#else
//	#error Qt library version 4.2 or higher is needed for this version of
// qps
#endif
    }

    //  QSize scaledSize = originalPixmap.size();
    //  scaledSize.scale(screenshotLabel->size(), Qt::KeepAspectRatio);
    //  if (!screenshotLabel->pixmap()|| scaledSize !=
    //  screenshotLabel->pixmap()->size())
    //    updateScreenshotLabel();
}

void Screenshot::newScreenshot()
{
    //    newScreenshotButton->setDisabled(true);
    //		shootScreen();
    //    QTimer::singleShot(delaySpinBox->value() * 1000, this,
    //    SLOT(shootScreen()));
}

void Screenshot::saveScreenshot()
{
    shootScreen();
    QString format = "png";
    QString path = QDir::homePath() + "/Desktop"; // *** ooooooo
    if (QFile::exists(path))
        path = path + tr("/untitled.") + format;
    else
        path = QDir::homePath();

    QString fileName = QFileDialog::getSaveFileName(
        this, tr("Save As"), path,
        tr("%1 Files (*.%2);;All Files (*)").arg(format.toUpper()).arg(format));
    if (!fileName.isEmpty())
        originalPixmap.save(fileName, format.toLatin1());

    //	setWindowFlags(Qt::X11BypassWindowManagerHint);
}

void Screenshot::shootScreen()
{

    //    if (delaySpinBox->value() != 0)  qApp->beep();
    originalPixmap = QPixmap::grabWindow(
        QApplication::desktop()->winId(), geometry().x() + shotarea->x(),
        geometry().y() + shotarea->y(), shotarea->width(), shotarea->height());
    ///   updateScreenshotLabel();
}

void Screenshot::updateCheckBox()
{
    if (delaySpinBox->value() == 0)
        hideThisWindowCheckBox->setDisabled(true);
    else
        hideThisWindowCheckBox->setDisabled(false);
}

void Screenshot::createOptionsGroupBox()
{
    optionsGroupBox = new QGroupBox(tr("Options"));

    delaySpinBox = new QSpinBox;
    delaySpinBox->setSuffix(tr(" s"));
    delaySpinBox->setMaximum(60);
    connect(delaySpinBox, SIGNAL(valueChanged(int)), this,
            SLOT(updateCheckBox()));

    delaySpinBoxLabel = new QLabel(tr("Screenshot Delay:"));

    hideThisWindowCheckBox = new QCheckBox(tr("Hide This Window"));

    optionsGroupBoxLayout = new QGridLayout;
    optionsGroupBoxLayout->addWidget(delaySpinBoxLabel, 0, 0);
    optionsGroupBoxLayout->addWidget(delaySpinBox, 0, 1);
    optionsGroupBoxLayout->addWidget(hideThisWindowCheckBox, 1, 0, 1, 2);
    optionsGroupBox->setLayout(optionsGroupBoxLayout);
}

void Screenshot::createButtonsLayout()
{
    newScreenshotButton =
        createButton(tr("New Screenshot"), this, SLOT(newScreenshot()));
    quitScreenshotButton = createButton(tr("Quit"), this, SLOT(close()));

    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(newScreenshotButton);
    buttonsLayout->addWidget(saveScreenshotButton);
    buttonsLayout->addWidget(quitScreenshotButton);
}

QPushButton *Screenshot::createButton(const QString &text, QWidget *receiver,
                                      const char *member)
{
    QPushButton *button = new QPushButton(text);
    button->connect(button, SIGNAL(clicked()), receiver, member);
    return button;
}

// preview
void Screenshot::updateScreenshotLabel()
{
    screenshotLabel->setPixmap(originalPixmap.scaled(screenshotLabel->size(),
                                                     Qt::KeepAspectRatio,
                                                     Qt::SmoothTransformation));
}

//#include "widget.h"

#include <QApplication>
#include <X11/extensions/Xrender.h>

int screenshot_main(int argc, char **argv)
// int screenshot_main()
{
    // qWarning("Please make sure you're running a composition manager!");
    bool argbVisual = false;
    Display *dpy = QX11Info::display(); // open default display
    if (!dpy)
    {
        qWarning("Cannot connect to the X server");
        exit(1);
    }

    int screen = DefaultScreen(dpy);
    Colormap colormap = 0;
    Visual *visual = 0;
    int eventBase, errorBase;

    if (0 and XRenderQueryExtension(dpy, &eventBase, &errorBase))
    {
        int nvi;
        XVisualInfo templ;
        templ.screen = screen;
        templ.depth = 32;
        templ.c_class = TrueColor;
        XVisualInfo *xvi = XGetVisualInfo(
            dpy, VisualScreenMask | VisualDepthMask | VisualClassMask, &templ,
            &nvi);

        printf("nvi=%d\n", nvi);
        for (int i = 0; i < nvi; ++i)
        {
            XRenderPictFormat *format =
                XRenderFindVisualFormat(dpy, xvi[i].visual);
            if (format->type == PictTypeDirect && format->direct.alphaMask > 0)
            {
                visual = xvi[i].visual;
                colormap = XCreateColormap(dpy, RootWindow(dpy, screen), visual,
                                           AllocNone);
                argbVisual = true;
                break;
            }
        }
    }
    if (argbVisual == true)
    {
        qWarning("Found ARGB visual. Starting app...");
        //    	QApplication app(dpy, argc, argv, Qt::HANDLE(visual),
        //    Qt::HANDLE(colormap));
    }
    else
    {
        // qWarning("Couldn't find ARGB visual... Exiting.");
        //    	QApplication app(dpy, argc, argv);
    }

    //    QApplication app(dpy, argc, argv, Qt::HANDLE(visual),
    //    Qt::HANDLE(colormap));
    QApplication app(argc, argv);

    Display *dsp = QX11Info::display(); // get the display(X server?)

    Screenshot *w = new Screenshot();
    w->show();
    // Widget w;
    // w.resize(400, 300);
    // w.show();

    return app.exec();
}
