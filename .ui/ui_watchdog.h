/********************************************************************************
** Form generated from reading UI file 'watchdog.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WATCHDOG_H
#define UI_WATCHDOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTableView>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_EventDialog
{
public:
    QVBoxLayout *vboxLayout;
    QListView *listView;
    QTableView *tableView;
    QGroupBox *groupBox_2;
    QVBoxLayout *vboxLayout1;
    QHBoxLayout *hboxLayout;
    QComboBox *comboBox;
    QLabel *labelDesc;
    QSpacerItem *spacerItem;
    QCheckBox *checkBox;
    QHBoxLayout *hboxLayout1;
    QLabel *label_procname;
    QLineEdit *proc_name;
    QLabel *label_cpu;
    QSpinBox *spinBox;
    QSpacerItem *spacerItem1;
    QCheckBox *checkBox_alreadyrun;
    QHBoxLayout *hboxLayout2;
    QLabel *label_3;
    QLineEdit *command;
    QHBoxLayout *hboxLayout3;
    QCheckBox *checkShowMsg;
    QLineEdit *message;
    QGroupBox *groupBox;
    QVBoxLayout *vboxLayout2;
    QLabel *label_2;
    QHBoxLayout *hboxLayout4;
    QPushButton *newButton;
    QPushButton *addButton;
    QPushButton *delButton;
    QPushButton *closeButton;

    void setupUi(QDialog *EventDialog)
    {
        if (EventDialog->objectName().isEmpty())
            EventDialog->setObjectName(QString::fromUtf8("EventDialog"));
        EventDialog->resize(680, 504);
        QSizePolicy sizePolicy(static_cast<QSizePolicy::Policy>(0), static_cast<QSizePolicy::Policy>(0));
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(EventDialog->sizePolicy().hasHeightForWidth());
        EventDialog->setSizePolicy(sizePolicy);
        EventDialog->setFocusPolicy(Qt::WheelFocus);
        vboxLayout = new QVBoxLayout(EventDialog);
#ifndef Q_OS_MAC
        vboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout->setObjectName(QString::fromUtf8("vboxLayout"));
        listView = new QListView(EventDialog);
        listView->setObjectName(QString::fromUtf8("listView"));
        sizePolicy.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy);
        listView->setMinimumSize(QSize(8, 1));
        listView->setMaximumSize(QSize(16777215, 51));

        vboxLayout->addWidget(listView);

        tableView = new QTableView(EventDialog);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setEnabled(true);
        QSizePolicy sizePolicy1(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(5));
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tableView->sizePolicy().hasHeightForWidth());
        tableView->setSizePolicy(sizePolicy1);
        tableView->setFocusPolicy(Qt::NoFocus);
        tableView->setContextMenuPolicy(Qt::NoContextMenu);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setShowGrid(false);
        tableView->setGridStyle(Qt::NoPen);

        vboxLayout->addWidget(tableView);

        groupBox_2 = new QGroupBox(EventDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy2(static_cast<QSizePolicy::Policy>(5), static_cast<QSizePolicy::Policy>(0));
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy2);
        vboxLayout1 = new QVBoxLayout(groupBox_2);
#ifndef Q_OS_MAC
        vboxLayout1->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        vboxLayout1->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout1->setObjectName(QString::fromUtf8("vboxLayout1"));
        hboxLayout = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
#ifndef Q_OS_MAC
        hboxLayout->setContentsMargins(0, 0, 0, 0);
#endif
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        comboBox = new QComboBox(groupBox_2);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        sizePolicy.setHeightForWidth(comboBox->sizePolicy().hasHeightForWidth());
        comboBox->setSizePolicy(sizePolicy);

        hboxLayout->addWidget(comboBox);

        labelDesc = new QLabel(groupBox_2);
        labelDesc->setObjectName(QString::fromUtf8("labelDesc"));

        hboxLayout->addWidget(labelDesc);

        spacerItem = new QSpacerItem(36, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        checkBox = new QCheckBox(groupBox_2);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setEnabled(false);

        hboxLayout->addWidget(checkBox);


        vboxLayout1->addLayout(hboxLayout);

        hboxLayout1 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout1->setSpacing(6);
#endif
        hboxLayout1->setContentsMargins(0, 0, 0, 0);
        hboxLayout1->setObjectName(QString::fromUtf8("hboxLayout1"));
        label_procname = new QLabel(groupBox_2);
        label_procname->setObjectName(QString::fromUtf8("label_procname"));

        hboxLayout1->addWidget(label_procname);

        proc_name = new QLineEdit(groupBox_2);
        proc_name->setObjectName(QString::fromUtf8("proc_name"));

        hboxLayout1->addWidget(proc_name);

        label_cpu = new QLabel(groupBox_2);
        label_cpu->setObjectName(QString::fromUtf8("label_cpu"));

        hboxLayout1->addWidget(label_cpu);

        spinBox = new QSpinBox(groupBox_2);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setValue(90);

        hboxLayout1->addWidget(spinBox);

        spacerItem1 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout1->addItem(spacerItem1);

        checkBox_alreadyrun = new QCheckBox(groupBox_2);
        checkBox_alreadyrun->setObjectName(QString::fromUtf8("checkBox_alreadyrun"));
        checkBox_alreadyrun->setEnabled(false);
        QFont font;
        checkBox_alreadyrun->setFont(font);
        checkBox_alreadyrun->setChecked(true);

        hboxLayout1->addWidget(checkBox_alreadyrun);


        vboxLayout1->addLayout(hboxLayout1);

        hboxLayout2 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout2->setSpacing(6);
#endif
        hboxLayout2->setContentsMargins(0, 0, 0, 0);
        hboxLayout2->setObjectName(QString::fromUtf8("hboxLayout2"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        hboxLayout2->addWidget(label_3);

        command = new QLineEdit(groupBox_2);
        command->setObjectName(QString::fromUtf8("command"));

        hboxLayout2->addWidget(command);


        vboxLayout1->addLayout(hboxLayout2);

        hboxLayout3 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout3->setSpacing(6);
#endif
        hboxLayout3->setContentsMargins(0, 0, 0, 0);
        hboxLayout3->setObjectName(QString::fromUtf8("hboxLayout3"));
        checkShowMsg = new QCheckBox(groupBox_2);
        checkShowMsg->setObjectName(QString::fromUtf8("checkShowMsg"));

        hboxLayout3->addWidget(checkShowMsg);

        message = new QLineEdit(groupBox_2);
        message->setObjectName(QString::fromUtf8("message"));

        hboxLayout3->addWidget(message);


        vboxLayout1->addLayout(hboxLayout3);


        vboxLayout->addWidget(groupBox_2);

        groupBox = new QGroupBox(EventDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        vboxLayout2 = new QVBoxLayout(groupBox);
        vboxLayout2->setSpacing(4);
#ifndef Q_OS_MAC
        vboxLayout2->setContentsMargins(9, 9, 9, 9);
#endif
        vboxLayout2->setObjectName(QString::fromUtf8("vboxLayout2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        vboxLayout2->addWidget(label_2);


        vboxLayout->addWidget(groupBox);

        hboxLayout4 = new QHBoxLayout();
#ifndef Q_OS_MAC
        hboxLayout4->setSpacing(6);
#endif
        hboxLayout4->setContentsMargins(0, 0, 0, 0);
        hboxLayout4->setObjectName(QString::fromUtf8("hboxLayout4"));
        newButton = new QPushButton(EventDialog);
        newButton->setObjectName(QString::fromUtf8("newButton"));

        hboxLayout4->addWidget(newButton);

        addButton = new QPushButton(EventDialog);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        hboxLayout4->addWidget(addButton);

        delButton = new QPushButton(EventDialog);
        delButton->setObjectName(QString::fromUtf8("delButton"));

        hboxLayout4->addWidget(delButton);

        closeButton = new QPushButton(EventDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));

        hboxLayout4->addWidget(closeButton);


        vboxLayout->addLayout(hboxLayout4);


        retranslateUi(EventDialog);

        QMetaObject::connectSlotsByName(EventDialog);
    } // setupUi

    void retranslateUi(QDialog *EventDialog)
    {
        EventDialog->setWindowTitle(QApplication::translate("EventDialog", "Watchdog 0.1 alpha", 0, QApplication::UnicodeUTF8));
        EventDialog->setStyleSheet(QApplication::translate("EventDialog", "QPushButtonX {\n"
"  /*   border: 2px solid #8f8f91;\n"
"   min-width: 80px; */\n"
"	/* image: url(:/icon/vista.png);*/\n"
"	color: rgb(244, 244, 244);\n"
"	border-image: url(:/icon/vista.png);\n"
" }\n"
"QWidget {\n"
"	\n"
"/*	color: rgb(255, 255, 244);*/\n"
"  /*   border: 2px solid green;\n"
"     border-radius: 4px;\n"
"     padding: 2px;*/\n"
"/*	background-color: rgb(66, 66, 66);*/\n"
"   /*  background-image: url(images/welcome.png);\n"
"	\n"
"	border-image: url(:/icon/vista.png);*/\n"
" } \n"
"QFrame, QLabel, QToolTip {\n"
"   /*  border: 2px solid green;\n"
"     border-radius: 4px;\n"
"     padding: 2px;\n"
"     background-image: url(images/welcome.png);*/\n"
" } \n"
" ", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("EventDialog", "Eventcat", 0, QApplication::UnicodeUTF8));
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("EventDialog", "Select condition", 0, QApplication::UnicodeUTF8)
        );
        labelDesc->setText(QApplication::translate("EventDialog", "labelDescrition", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("EventDialog", "Enable", 0, QApplication::UnicodeUTF8));
        label_procname->setText(QApplication::translate("EventDialog", "process name", 0, QApplication::UnicodeUTF8));
        label_cpu->setText(QApplication::translate("EventDialog", "cpu", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        spinBox->setToolTip(QApplication::translate("EventDialog", "acdcd", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_STATUSTIP
        spinBox->setStatusTip(QApplication::translate("EventDialog", "ddddd", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_STATUSTIP
#ifndef QT_NO_WHATSTHIS
        spinBox->setWhatsThis(QApplication::translate("EventDialog", "ccccc", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
#ifndef QT_NO_ACCESSIBILITY
        spinBox->setAccessibleName(QApplication::translate("EventDialog", "aaaa", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_ACCESSIBILITY
        spinBox->setSuffix(QApplication::translate("EventDialog", "%", 0, QApplication::UnicodeUTF8));
        spinBox->setPrefix(QString());
        checkBox_alreadyrun->setText(QApplication::translate("EventDialog", "include already running process", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("EventDialog", "run command", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        command->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        checkShowMsg->setText(QApplication::translate("EventDialog", "show Message", 0, QApplication::UnicodeUTF8));
        message->setText(QString());
        groupBox->setTitle(QApplication::translate("EventDialog", "Help (Not yet. just concept)", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("EventDialog", "%p : pid\n"
"%c : command", 0, QApplication::UnicodeUTF8));
        newButton->setStyleSheet(QString());
        newButton->setText(QApplication::translate("EventDialog", "New", 0, QApplication::UnicodeUTF8));
        addButton->setText(QApplication::translate("EventDialog", "Add", 0, QApplication::UnicodeUTF8));
        delButton->setText(QApplication::translate("EventDialog", "Delete", 0, QApplication::UnicodeUTF8));
        closeButton->setText(QApplication::translate("EventDialog", "Close", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class EventDialog: public Ui_EventDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WATCHDOG_H
