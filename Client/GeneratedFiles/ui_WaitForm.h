/********************************************************************************
** Form generated from reading ui file 'WaitForm.ui'
**
** Created: Tue 14. Aug 16:17:42 2012
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_WAITFORM_H
#define UI_WAITFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaitFormClass
{
public:
    QGridLayout *gridLayout;
    QLabel *lTimeWait;
    QPushButton *bExitQueue;
    QLabel *lCntWait;
    QLabel *lFight;
    QLabel *lCntCommon;

    void setupUi(QWidget *WaitFormClass)
    {
        if (WaitFormClass->objectName().isEmpty())
            WaitFormClass->setObjectName(QString::fromUtf8("WaitFormClass"));
        WaitFormClass->resize(210, 148);
        gridLayout = new QGridLayout(WaitFormClass);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lTimeWait = new QLabel(WaitFormClass);
        lTimeWait->setObjectName(QString::fromUtf8("lTimeWait"));

        gridLayout->addWidget(lTimeWait, 0, 0, 1, 1);

        bExitQueue = new QPushButton(WaitFormClass);
        bExitQueue->setObjectName(QString::fromUtf8("bExitQueue"));

        gridLayout->addWidget(bExitQueue, 1, 0, 1, 1);

        lCntWait = new QLabel(WaitFormClass);
        lCntWait->setObjectName(QString::fromUtf8("lCntWait"));

        gridLayout->addWidget(lCntWait, 2, 0, 1, 1);

        lFight = new QLabel(WaitFormClass);
        lFight->setObjectName(QString::fromUtf8("lFight"));

        gridLayout->addWidget(lFight, 3, 0, 1, 1);

        lCntCommon = new QLabel(WaitFormClass);
        lCntCommon->setObjectName(QString::fromUtf8("lCntCommon"));

        gridLayout->addWidget(lCntCommon, 4, 0, 1, 1);


        retranslateUi(WaitFormClass);

        QMetaObject::connectSlotsByName(WaitFormClass);
    } // setupUi

    void retranslateUi(QWidget *WaitFormClass)
    {
        WaitFormClass->setWindowTitle(QApplication::translate("WaitFormClass", "Form", 0, QApplication::UnicodeUTF8));
        lTimeWait->setText(QApplication::translate("WaitFormClass", "\320\222\321\200\320\265\320\274\321\217 \320\276\320\266\320\270\320\264\320\260\320\275\320\270\321\217:", 0, QApplication::UnicodeUTF8));
        bExitQueue->setText(QApplication::translate("WaitFormClass", "\320\222\321\213\320\271\321\202\320\270 \320\270\320\267 \320\276\321\207\320\265\321\200\320\265\320\264\320\270", 0, QApplication::UnicodeUTF8));
        lCntWait->setText(QApplication::translate("WaitFormClass", "\320\232\320\276\320\273-\320\262\320\276 \320\276\320\266\320\270\320\264\320\260\321\216\321\211\320\270\321\205", 0, QApplication::UnicodeUTF8));
        lFight->setText(QApplication::translate("WaitFormClass", "\320\222 \320\261\320\276\321\216", 0, QApplication::UnicodeUTF8));
        lCntCommon->setText(QApplication::translate("WaitFormClass", "\320\236\320\261\321\211\320\265\320\265 \320\272\320\276\320\273-\320\262\320\276", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(WaitFormClass);
    } // retranslateUi

};

namespace Ui {
    class WaitFormClass: public Ui_WaitFormClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITFORM_H
