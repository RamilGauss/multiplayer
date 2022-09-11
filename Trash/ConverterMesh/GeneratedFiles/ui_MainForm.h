/********************************************************************************
** Form generated from reading ui file 'MainForm.ui'
**
** Created: Thu 30. Aug 19:39:54 2012
**      by: Qt User Interface Compiler version 4.5.0
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINFORM_H
#define UI_MAINFORM_H

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
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FormMainForm
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lePath;
    QPushButton *bSetPath;
    QPushButton *bStart;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *FormMainForm)
    {
        if (FormMainForm->objectName().isEmpty())
            FormMainForm->setObjectName(QString::fromUtf8("FormMainForm"));
        FormMainForm->resize(384, 67);
        gridLayout = new QGridLayout(FormMainForm);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(FormMainForm);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lePath = new QLineEdit(FormMainForm);
        lePath->setObjectName(QString::fromUtf8("lePath"));

        gridLayout->addWidget(lePath, 0, 1, 1, 2);

        bSetPath = new QPushButton(FormMainForm);
        bSetPath->setObjectName(QString::fromUtf8("bSetPath"));

        gridLayout->addWidget(bSetPath, 1, 0, 1, 1);

        bStart = new QPushButton(FormMainForm);
        bStart->setObjectName(QString::fromUtf8("bStart"));

        gridLayout->addWidget(bStart, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(201, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 2, 1, 1);


        retranslateUi(FormMainForm);

        QMetaObject::connectSlotsByName(FormMainForm);
    } // setupUi

    void retranslateUi(QWidget *FormMainForm)
    {
        FormMainForm->setWindowTitle(QApplication::translate("FormMainForm", "Form", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("FormMainForm", "\320\237\321\203\321\202\321\214 \321\201 Obj", 0, QApplication::UnicodeUTF8));
        bSetPath->setText(QApplication::translate("FormMainForm", "...", 0, QApplication::UnicodeUTF8));
        bStart->setText(QApplication::translate("FormMainForm", "\320\241\321\202\320\260\321\200\321\202", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(FormMainForm);
    } // retranslateUi

};

namespace Ui {
    class FormMainForm: public Ui_FormMainForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINFORM_H
