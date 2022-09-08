#include "ServerForm.h"
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QWindowsStyle>


#include "ApplicationProtocolPacket.h"
#include "ApplicationProtocolMainPacket.h"
#include "ErrorReg.h"
#include "HiTimer.h"
#include "NetSystem.h"

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
  ServerForm w;
  w.show();
  return a.exec();
}
