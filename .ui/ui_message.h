/********************************************************************************
** Form generated from reading UI file 'message.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGE_H
#define UI_MESSAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ExecWindow
{
public:
    QVBoxLayout *vboxLayout;
    QTextEdit *textEdit;
    QPushButton *okButton;

    void setupUi(QWidget *ExecWindow)
    {
        if (ExecWindow->objectName().isEmpty())
            ExecWindow->setObjectName(QString::fromUtf8("ExecWindow"));
        ExecWindow->resize(309, 162);
        QIcon icon;
        icon.addFile(QString::fromUtf8(""), QSize(), QIcon::Normal, QIcon::Off);
        ExecWindow->setWindowIcon(icon);
        vboxLayout = new QVBoxLayout(ExecWindow);
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        textEdit = new QTextEdit(ExecWindow);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        vboxLayout->addWidget(textEdit);

        okButton = new QPushButton(ExecWindow);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        vboxLayout->addWidget(okButton);


        retranslateUi(ExecWindow);

        QMetaObject::connectSlotsByName(ExecWindow);
    } // setupUi

    void retranslateUi(QWidget *ExecWindow)
    {
        ExecWindow->setWindowTitle(QApplication::translate("ExecWindow", "Qps", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("ExecWindow", "Ok", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ExecWindow: public Ui_ExecWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGE_H
