/********************************************************************************
** Form generated from reading ui file 'ServerForm.ui'
**
** Created: Wed 13. Jun 09:53:37 2012
**      by: Qt User Interface Compiler version 4.4.3
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_SERVERFORM_H
#define UI_SERVERFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QTableWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ServerFormClass
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableClient;
    QTableWidget *tableRoom;

    void setupUi(QWidget *ServerFormClass)
    {
    if (ServerFormClass->objectName().isEmpty())
        ServerFormClass->setObjectName(QString::fromUtf8("ServerFormClass"));
    ServerFormClass->resize(562, 658);
    gridLayout = new QGridLayout(ServerFormClass);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    tableClient = new QTableWidget(ServerFormClass);
    if (tableClient->columnCount() < 3)
        tableClient->setColumnCount(3);
    QTableWidgetItem *__colItem = new QTableWidgetItem();
    tableClient->setHorizontalHeaderItem(0, __colItem);
    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    tableClient->setHorizontalHeaderItem(1, __colItem1);
    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    tableClient->setHorizontalHeaderItem(2, __colItem2);
    if (tableClient->rowCount() < 2)
        tableClient->setRowCount(2);
    QTableWidgetItem *__rowItem = new QTableWidgetItem();
    tableClient->setVerticalHeaderItem(0, __rowItem);
    QTableWidgetItem *__rowItem1 = new QTableWidgetItem();
    tableClient->setVerticalHeaderItem(1, __rowItem1);
    tableClient->setObjectName(QString::fromUtf8("tableClient"));
    tableClient->setSelectionMode(QAbstractItemView::SingleSelection);
    tableClient->setSelectionBehavior(QAbstractItemView::SelectRows);

    gridLayout->addWidget(tableClient, 0, 0, 1, 1);

    tableRoom = new QTableWidget(ServerFormClass);
    if (tableRoom->columnCount() < 2)
        tableRoom->setColumnCount(2);
    QTableWidgetItem *__colItem3 = new QTableWidgetItem();
    tableRoom->setHorizontalHeaderItem(0, __colItem3);
    QTableWidgetItem *__colItem4 = new QTableWidgetItem();
    tableRoom->setHorizontalHeaderItem(1, __colItem4);
    if (tableRoom->rowCount() < 2)
        tableRoom->setRowCount(2);
    QTableWidgetItem *__rowItem2 = new QTableWidgetItem();
    tableRoom->setVerticalHeaderItem(0, __rowItem2);
    QTableWidgetItem *__rowItem3 = new QTableWidgetItem();
    tableRoom->setVerticalHeaderItem(1, __rowItem3);
    tableRoom->setObjectName(QString::fromUtf8("tableRoom"));
    tableRoom->setSelectionMode(QAbstractItemView::SingleSelection);
    tableRoom->setSelectionBehavior(QAbstractItemView::SelectRows);

    gridLayout->addWidget(tableRoom, 1, 0, 1, 1);


    retranslateUi(ServerFormClass);

    QMetaObject::connectSlotsByName(ServerFormClass);
    } // setupUi

    void retranslateUi(QWidget *ServerFormClass)
    {
    ServerFormClass->setWindowTitle(QApplication::translate("ServerFormClass", "Form", 0, QApplication::UnicodeUTF8));
    tableClient->horizontalHeaderItem(0)->setText(QApplication::translate("ServerFormClass", "\320\230\320\274\321\217", 0, QApplication::UnicodeUTF8));
    tableClient->horizontalHeaderItem(1)->setText(QApplication::translate("ServerFormClass", "\320\241\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
    tableClient->horizontalHeaderItem(2)->setText(QApplication::translate("ServerFormClass", "IP:Port", 0, QApplication::UnicodeUTF8));
    tableClient->verticalHeaderItem(0)->setText(QApplication::translate("ServerFormClass", "Gwird", 0, QApplication::UnicodeUTF8));
    tableClient->verticalHeaderItem(1)->setText(QApplication::translate("ServerFormClass", "Gauss", 0, QApplication::UnicodeUTF8));
    tableRoom->horizontalHeaderItem(0)->setText(QApplication::translate("ServerFormClass", "\320\236\321\201\321\202\320\260\321\202\320\276\320\272 \320\262\321\200\320\265\320\274\320\265\320\275\320\270", 0, QApplication::UnicodeUTF8));
    tableRoom->horizontalHeaderItem(1)->setText(QApplication::translate("ServerFormClass", "\320\232\320\276\320\273-\320\262\320\276 \320\260\320\272\321\202\320\270\320\262\320\275\321\213\321\205", 0, QApplication::UnicodeUTF8));
    tableRoom->verticalHeaderItem(0)->setText(QApplication::translate("ServerFormClass", "Gwird", 0, QApplication::UnicodeUTF8));
    tableRoom->verticalHeaderItem(1)->setText(QApplication::translate("ServerFormClass", "Gauss", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ServerFormClass);
    } // retranslateUi

};

namespace Ui {
    class ServerFormClass: public Ui_ServerFormClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERFORM_H
