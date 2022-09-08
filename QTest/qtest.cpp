#include "qtest.h"

QTest::QTest(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
  ui.setupUi(this);
}

QTest::~QTest()
{

}
