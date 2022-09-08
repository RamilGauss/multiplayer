#include <Qt/qapplication.h>
#include <QtGui/QtGui>
#include <Qt/qlabel.h> 
#include <qt/qtimer.h>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QWidget *pW= new QWidget;
  pW->show();
  return app.exec();
}