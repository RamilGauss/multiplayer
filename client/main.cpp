#include "ClientMain.h"
#include <QtGui/QApplication>

#include <QtCore/QTextCodec>
#include <QWindowsStyle>
#include "HiTimer.h"
#include "NetSystem.h"
#include "ErrorReg.h"
#include <glib/gthread.h>
#include "ManagerGUI.h"


#include "ApplicationProtocolPacketAnswer.h"//####


int main(int argc, char *argv[])
{
  g_thread_init( NULL );
  err_Init();
  errSTR_Init();
  errSTD_Init();
  errSDK_Init();
  ht_Init();
  ns_Init();

  QTextCodec::setCodecForTr(QTextCodec::codecForName("CP1251"));

  QApplication a(argc, argv);
  TManagerGUI managerGUI;
  managerGUI.start();

  if(argc==4)
  {
    char* sNick = argv[1];
    char*   sIP = argv[2]; 
    char*  port = argv[3]; 
    managerGUI.Connect(sNick,sIP,port);
  }

	a.exec();

	return 0;
}
