#ifndef QTEST_H
#define QTEST_H

#include <QtGui/QMainWindow>
#include "ui_qtest.h"

class QTest : public QMainWindow
{
    Q_OBJECT

public:
    QTest(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~QTest();

private:
    Ui::QTestClass ui;
};

#endif // QTEST_H
