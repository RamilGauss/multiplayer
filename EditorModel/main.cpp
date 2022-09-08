#include <QtGui/QWidget> 
#include <QtGui/QApplication> 
#include <QtGui/QHBoxLayout> 
#include "Direct3DWidget.h" 
#include <QtCore/QTextCodec>
#include <QWindowsStyle>


int main(int argc, char *argv[]) 
{ 
  QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));

  QApplication xApplication(argc, argv);
  //--------------------------------Initialization complete 
  QWidget xMainWindow; 
  TBaseGUI_DX* xScene = new TBaseGUI_DX(&xMainWindow); 
  QHBoxLayout* xMainLayout = new QHBoxLayout; 
  xMainLayout->addWidget(xScene); 
  xMainWindow.setLayout(xMainLayout); 
  //It is important to initialize the Direct3d after the widget was embedded to the window 
  xMainWindow.show(); 

  return xApplication.exec(); 
} 
