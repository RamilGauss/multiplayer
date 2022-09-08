/********************************************************************************
** Form generated from reading ui file 'editormodel.ui'
**
** Created: Wed 8. Aug 15:32:32 2012
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_EDITORMODEL_H
#define UI_EDITORMODEL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditorModelClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *EditorModelClass)
    {
        if (EditorModelClass->objectName().isEmpty())
            EditorModelClass->setObjectName(QString::fromUtf8("EditorModelClass"));
        EditorModelClass->resize(600, 400);
        menuBar = new QMenuBar(EditorModelClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        EditorModelClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(EditorModelClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        EditorModelClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(EditorModelClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        EditorModelClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(EditorModelClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        EditorModelClass->setStatusBar(statusBar);

        retranslateUi(EditorModelClass);

        QMetaObject::connectSlotsByName(EditorModelClass);
    } // setupUi

    void retranslateUi(QMainWindow *EditorModelClass)
    {
        EditorModelClass->setWindowTitle(QApplication::translate("EditorModelClass", "EditorModel", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(EditorModelClass);
    } // retranslateUi

};

namespace Ui {
    class EditorModelClass: public Ui_EditorModelClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORMODEL_H
