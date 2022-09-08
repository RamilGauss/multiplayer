/********************************************************************************
** Form generated from reading ui file 'ServerForm.ui'
**
** Created: Fri 4. May 15:10:32 2012
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
    QTableWidget *table;

    void setupUi(QWidget *ServerFormClass)
    {
    if (ServerFormClass->objectName().isEmpty())
        ServerFormClass->setObjectName(QString::fromUtf8("ServerFormClass"));
    ServerFormClass->resize(562, 658);
    gridLayout = new QGridLayout(ServerFormClass);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    table = new QTableWidget(ServerFormClass);
    if (table->columnCount() < 3)
        table->setColumnCount(3);
    QTableWidgetItem *__colItem = new QTableWidgetItem();
    table->setHorizontalHeaderItem(0, __colItem);
    QTableWidgetItem *__colItem1 = new QTableWidgetItem();
    table->setHorizontalHeaderItem(1, __colItem1);
    QTableWidgetItem *__colItem2 = new QTableWidgetItem();
    table->setHorizontalHeaderItem(2, __colItem2);
    if (table->rowCount() < 2)
        table->setRowCount(2);
    QTableWidgetItem *__rowItem = new QTableWidgetItem();
    table->setVerticalHeaderItem(0, __rowItem);
    QTableWidgetItem *__rowItem1 = new QTableWidgetItem();
    table->setVerticalHeaderItem(1, __rowItem1);
    table->setObjectName(QString::fromUtf8("table"));
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    gridLayout->addWidget(table, 0, 0, 1, 1);


    retranslateUi(ServerFormClass);

    QMetaObject::connectSlotsByName(ServerFormClass);
    } // setupUi

    void retranslateUi(QWidget *ServerFormClass)
    {
    ServerFormClass->setWindowTitle(QApplication::translate("ServerFormClass", "Form", 0, QApplication::UnicodeUTF8));
    table->horizontalHeaderItem(0)->setText(QApplication::translate("ServerFormClass", "\320\230\320\274\321\217", 0, QApplication::UnicodeUTF8));
    table->horizontalHeaderItem(1)->setText(QApplication::translate("ServerFormClass", "\320\241\320\276\321\201\321\202\320\276\321\217\320\275\320\270\320\265", 0, QApplication::UnicodeUTF8));
    table->horizontalHeaderItem(2)->setText(QApplication::translate("ServerFormClass", "IP:Port", 0, QApplication::UnicodeUTF8));
    table->verticalHeaderItem(0)->setText(QApplication::translate("ServerFormClass", "Gwird", 0, QApplication::UnicodeUTF8));
    table->verticalHeaderItem(1)->setText(QApplication::translate("ServerFormClass", "Gauss", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(ServerFormClass);
    } // retranslateUi

};

namespace Ui {
    class ServerFormClass: public Ui_ServerFormClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERFORM_H
