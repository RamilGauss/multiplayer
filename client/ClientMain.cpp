#include "ClientMain.h"
#include "NetSystem.h"
#include "ClientTank.h"
#include "ApplicationProtocolDef.h"
#include "GameRoomPrepare.h"
#include "TransportProtocolTank.h"
#include <QMessageBox>
#include <QSettings>
#include "ApplicationProtocolPacketAnswer.h"
#include "WaitForm.h"
#include "GameForm.h"

//-------------------------------------------------------------------------------------------
ClientMain::ClientMain(QWidget *parent)
: TBaseGUI(parent)
{
  ui.setupUi(this);

  setWindowTitle(tr(STR_VERSION_CLIENT));

  connect(ui.bEnter, SIGNAL(clicked()), this, SLOT(sl_Enter()));
  connect(ui.bExit,  SIGNAL(clicked()), this, SLOT(sl_Exit()));

  LoadFromReestr();
}
//---------------------------------------------------------------------------------------------
ClientMain::~ClientMain()
{
}
//---------------------------------------------------------------------------------------------
void ClientMain::sl_Exit()
{
  _exit(0);
}
//---------------------------------------------------------------------------------------------
void ClientMain::closeEvent(QCloseEvent* )
{
	sl_Exit();
}
//---------------------------------------------------------------------------------------------
void ClientMain::sl_Enter()
{
  bool ok;
  
  QByteArray baIP = ui.leIPServer->text().toLocal8Bit();
  char *sIP = (char*)baIP.data();

  unsigned int ip = ns_inet_addr(sIP);
  
  unsigned int port = ui.leLocalPort->text().toInt(&ok);
  if(ok==false) {Q_MESSAGE(tr("Некорректный ввод локального порта."));return;}
  
  QString nick = ui.leNik->text();
  if(nick.length()==0){Q_MESSAGE(tr("Не задан ник"));return;}
  //-------------------------------
  // сохранить в реестр введенные параметры
  SaveInReestr(ip,port,nick);
  //-------------------------------
  // осуществить вход
  QByteArray baNick = nick.toLocal8Bit();
  char* sNick = baNick.data();
  if(!GlobalClientTank.Connect(ip,port,sNick))
		Q_MESSAGE(tr("Нет связи с сервером."));
}
//---------------------------------------------------------------------------------------------
void ClientMain::SaveInReestr(unsigned int ip, unsigned int port,QString& nick)
{
	QSettings settings("RUSSIA","ClientMain");
	settings.setValue("ip", ip);
	settings.setValue("port", port);
	settings.setValue("nick", nick);
}
//---------------------------------------------------------------------------------------------
void ClientMain::LoadFromReestr()
{
	QSettings settings("RUSSIA","ClientMain");
	bool ok = false;
	unsigned int ip   = settings.value("ip",  0).toUInt(&ok);
	unsigned int port = settings.value("port",0).toUInt(&ok);
	QString nick      = settings.value("nick",QString()).toString();
	if(ok==false) return;

	char* sIP = ns_str_addr(ip);
	ui.leIPServer->setText(sIP);

	char s[100];
	sprintf(s,"%u",port);
	ui.leLocalPort->setText(s);

	ui.leNik->setText(nick);
}
//---------------------------------------------------------------------------------------------
void ClientMain::Translate( unsigned short type,char *pData, int size )
{
  // пустышка
}
//---------------------------------------------------------------------------------------------
void ClientMain::Connect(char* sNick, char* ip,char* port)// поддержка скрипт-управления
{
  ui.leNik->setText(tr(sNick));
  ui.leLocalPort->setText(tr(port));
  ui.leIPServer->setText(tr(ip));
  sl_Enter();
}
//---------------------------------------------------------------------------------------------
