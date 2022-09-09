/********************************************************************************
** Form generated from reading ui file 'EditorModel.ui'
**
** Created: Fri 2. Nov 10:53:51 2012
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
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormEditorModel
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;

    void setupUi(QWidget *FormEditorModel)
    {
        if (FormEditorModel->objectName().isEmpty())
            FormEditorModel->setObjectName(QString::fromUtf8("FormEditorModel"));
        FormEditorModel->resize(400, 300);
        gridLayout = new QGridLayout(FormEditorModel);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pushButton = new QPushButton(FormEditorModel);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);


        retranslateUi(FormEditorModel);

        QMetaObject::connectSlotsByName(FormEditorModel);
    } // setupUi

    void retranslateUi(QWidget *FormEditorModel)
    {
        FormEditorModel->setWindowTitle(QApplication::translate("FormEditorModel", "Form", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("FormEditorModel", "PushButton", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(FormEditorModel);
    } // retranslateUi

};

namespace Ui {
    class FormEditorModel: public Ui_FormEditorModel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITORMODEL_H
