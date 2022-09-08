#include <QtGui/QWidget> 
#include <QtGui/QApplication> 
#include <QtGui/QHBoxLayout> 
#include "Direct3DWidget.h" 
#include <QtCore/QTextCodec>
#include <QWindowsStyle>


int main(int argc, char *argv[]) 
{ 
#if 0
  //------------------------------Start initialization 
  //Additional parameters, just add a parameter separated by comma 
  std::string aAdditionalParameters[] = {"-direct3d"}; 
  int iRealArgumentAmount = argc + sizeof(aAdditionalParameters)/sizeof(std::string); 
  //This double pointer will contain parameters from argv and 
  //statical parameters which necessary for this application 
  char** pArguments = new char*[iRealArgumentAmount]; 
  //Copy all source(from the command line) parameters 
  for(int i = 0; i < argc; ++i) 
  { 
    *(pArguments + i) = new char[ strlen(argv[i]) + 1 ]; 
    strcpy( *(pArguments + i), argv[i] ); 
  } 
  //Append parameters we want to add 
  for(int i = argc, j = 0; i < iRealArgumentAmount; ++i, ++j) 
  { 
    *(pArguments + i) = new char[ aAdditionalParameters[j].size() + 1 ]; 
    strcpy( *(pArguments + i), aAdditionalParameters[j].c_str() ); 
  } 
  QApplication xApplication(iRealArgumentAmount, pArguments); 
  for(int i = 0; i < iRealArgumentAmount; ++i) 
    delete []*(pArguments + i); 
  delete []pArguments; 
#else
  QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));

  QApplication xApplication(argc, argv);
#endif
  //--------------------------------Initialization complete 
  QWidget xMainWindow; 
  QD3DWidget* xScene = new QD3DWidget(&xMainWindow); 
  QHBoxLayout* xMainLayout = new QHBoxLayout; 
  xMainLayout->addWidget(xScene); 
  xMainWindow.setLayout(xMainLayout); 
  //It is important to initialize the Direct3d after the widget was embedded to the window 
  xMainWindow.show(); 

  return xApplication.exec(); 
} 
