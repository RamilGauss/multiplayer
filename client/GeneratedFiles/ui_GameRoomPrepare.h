/********************************************************************************
** Form generated from reading ui file 'GameRoomPrepare.ui'
**
** Created: Fri 11. May 16:30:21 2012
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_GAMEROOMPREPARE_H
#define UI_GAMEROOMPREPARE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameRoomPrepareClass
{
public:
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QPushButton *bFight;
    QPushButton *bExit;
    QLabel *label;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *lInFight;
    QLabel *lAll;
    QComboBox *cbTank;
    QLabel *lState;

    void setupUi(QWidget *GameRoomPrepareClass)
    {
    if (GameRoomPrepareClass->objectName().isEmpty())
        GameRoomPrepareClass->setObjectName(QString::fromUtf8("GameRoomPrepareClass"));
    GameRoomPrepareClass->resize(225, 137);
    gridLayout_3 = new QGridLayout(GameRoomPrepareClass);
    gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
    gridLayout_2 = new QGridLayout();
    gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
    bFight = new QPushButton(GameRoomPrepareClass);
    bFight->setObjectName(QString::fromUtf8("bFight"));

    gridLayout_2->addWidget(bFight, 0, 0, 1, 1);

    bExit = new QPushButton(GameRoomPrepareClass);
    bExit->setObjectName(QString::fromUtf8("bExit"));

    gridLayout_2->addWidget(bExit, 0, 1, 1, 2);

    label = new QLabel(GameRoomPrepareClass);
    label->setObjectName(QString::fromUtf8("label"));

    gridLayout_2->addWidget(label, 1, 0, 1, 2);

    groupBox = new QGroupBox(GameRoomPrepareClass);
    groupBox->setObjectName(QString::fromUtf8("groupBox"));
    gridLayout = new QGridLayout(groupBox);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    lInFight = new QLabel(groupBox);
    lInFight->setObjectName(QString::fromUtf8("lInFight"));

    gridLayout->addWidget(lInFight, 0, 0, 1, 1);

    lAll = new QLabel(groupBox);
    lAll->setObjectName(QString::fromUtf8("lAll"));

    gridLayout->addWidget(lAll, 1, 0, 1, 1);


    gridLayout_2->addWidget(groupBox, 1, 2, 2, 1);

    cbTank = new QComboBox(GameRoomPrepareClass);
    cbTank->setObjectName(QString::fromUtf8("cbTank"));

    gridLayout_2->addWidget(cbTank, 2, 0, 1, 2);


    gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 1);

    lState = new QLabel(GameRoomPrepareClass);
    lState->setObjectName(QString::fromUtf8("lState"));

    gridLayout_3->addWidget(lState, 1, 0, 1, 1);


    retranslateUi(GameRoomPrepareClass);

    QMetaObject::connectSlotsByName(GameRoomPrepareClass);
    } // setupUi

    void retranslateUi(QWidget *GameRoomPrepareClass)
    {
    GameRoomPrepareClass->setWindowTitle(QApplication::translate("GameRoomPrepareClass", "Form", 0, QApplication::UnicodeUTF8));
    bFight->setText(QApplication::translate("GameRoomPrepareClass", "\320\222 \320\261\320\276\320\271", 0, QApplication::UnicodeUTF8));
    bExit->setText(QApplication::translate("GameRoomPrepareClass", "\320\222\321\213\321\205\320\276\320\264", 0, QApplication::UnicodeUTF8));
    label->setText(QApplication::translate("GameRoomPrepareClass", "\320\222\321\213\320\261\320\276\321\200 \321\202\320\265\321\205\320\275\320\270\320\272\320\270", 0, QApplication::UnicodeUTF8));
    groupBox->setTitle(QApplication::translate("GameRoomPrepareClass", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", 0, QApplication::UnicodeUTF8));
    lInFight->setText(QApplication::translate("GameRoomPrepareClass", "\320\222 \320\261\320\276\321\216:", 0, QApplication::UnicodeUTF8));
    lAll->setText(QApplication::translate("GameRoomPrepareClass", "\320\222\321\201\320\265\320\263\320\276:", 0, QApplication::UnicodeUTF8));
    cbTank->clear();
    cbTank->insertItems(0, QStringList()
     << QApplication::translate("GameRoomPrepareClass", "\320\241\320\242", 0, QApplication::UnicodeUTF8)
     << QApplication::translate("GameRoomPrepareClass", "\320\242\320\242", 0, QApplication::UnicodeUTF8)
    );
    lState->setText(QString());
    Q_UNUSED(GameRoomPrepareClass);
    } // retranslateUi

};

namespace Ui {
    class GameRoomPrepareClass: public Ui_GameRoomPrepareClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEROOMPREPARE_H
