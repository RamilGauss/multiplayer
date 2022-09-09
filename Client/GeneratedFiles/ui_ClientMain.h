/********************************************************************************
** Form generated from reading ui file 'ClientMain.ui'
**
** Created: Sat 15. Sep 21:02:17 2012
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CLIENTMAIN_H
#define UI_CLIENTMAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ClientMainClass
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *leIPServer;
    QLabel *label_2;
    QLineEdit *leLocalPort;
    QLabel *label_3;
    QLineEdit *leNik;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QPushButton *bEnter;
    QPushButton *bExit;
    QSpacerItem *horizontalSpacer_2;

    void setupUi(QWidget *ClientMainClass)
    {
        if (ClientMainClass->objectName().isEmpty())
            ClientMainClass->setObjectName(QString::fromUtf8("ClientMainClass"));
        ClientMainClass->resize(284, 150);
        gridLayout = new QGridLayout(ClientMainClass);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(-1, 9, -1, 3);
        label = new QLabel(ClientMainClass);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 2);

        leIPServer = new QLineEdit(ClientMainClass);
        leIPServer->setObjectName(QString::fromUtf8("leIPServer"));

        gridLayout->addWidget(leIPServer, 0, 2, 1, 2);

        label_2 = new QLabel(ClientMainClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 2);

        leLocalPort = new QLineEdit(ClientMainClass);
        leLocalPort->setObjectName(QString::fromUtf8("leLocalPort"));

        gridLayout->addWidget(leLocalPort, 1, 2, 1, 2);

        label_3 = new QLabel(ClientMainClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 2);

        leNik = new QLineEdit(ClientMainClass);
        leNik->setObjectName(QString::fromUtf8("leNik"));

        gridLayout->addWidget(leNik, 2, 2, 1, 2);

        horizontalSpacer = new QSpacerItem(75, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        bEnter = new QPushButton(ClientMainClass);
        bEnter->setObjectName(QString::fromUtf8("bEnter"));

        verticalLayout->addWidget(bEnter);

        bExit = new QPushButton(ClientMainClass);
        bExit->setObjectName(QString::fromUtf8("bExit"));

        verticalLayout->addWidget(bExit);


        gridLayout->addLayout(verticalLayout, 3, 1, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(96, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 3, 3, 1, 1);


        retranslateUi(ClientMainClass);

        QMetaObject::connectSlotsByName(ClientMainClass);
    } // setupUi

    void retranslateUi(QWidget *ClientMainClass)
    {
        ClientMainClass->setWindowTitle(QApplication::translate("ClientMainClass", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("ClientMainClass", "\320\220\320\264\321\200\320\265\321\201 \321\201\320\265\321\200\320\262\320\265\321\200\320\260", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("ClientMainClass", "\320\233\320\276\320\272\320\260\320\273\321\214\320\275\321\213\320\271 \320\277\320\276\321\200\321\202", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("ClientMainClass", "\320\235\320\270\320\272", 0, QApplication::UnicodeUTF8));
        bEnter->setText(QApplication::translate("ClientMainClass", "\320\222\320\276\320\271\321\202\320\270", 0, QApplication::UnicodeUTF8));
        bExit->setText(QApplication::translate("ClientMainClass", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ClientMainClass);
    } // retranslateUi

};

namespace Ui {
    class ClientMainClass: public Ui_ClientMainClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTMAIN_H
