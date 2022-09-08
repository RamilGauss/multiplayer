/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "ClientMain.h"
#include "NetSystem.h"
#include "ApplicationProtocolDef.h"
#include <QMessageBox>
#include <QSettings>
#include "ApplicationProtocolPacketAnswer.h"
#include "ClientTank.h"

//-------------------------------------------------------------------------------------------
TClientMain::TClientMain(QWidget *parent)
: TBaseGUI(parent)
{
  ui.setupUi(this);

  setWindowTitle(tr(STR_VERSION_CLIENT));

  connect(ui.bEnter, SIGNAL(clicked()), this, SLOT(sl_Enter()));
  connect(ui.bExit,  SIGNAL(clicked()), this, SLOT(sl_Exit()));

  LoadFromReestr();
}
//---------------------------------------------------------------------------------------------
TClientMain::~TClientMain()
{
}
//---------------------------------------------------------------------------------------------
void TClientMain::sl_Exit()
{
  close();//_exit(0);
}
//---------------------------------------------------------------------------------------------
//void TClientMain::closeEvent(QCloseEvent* )
//{
	//sl_Exit();
//}
//---------------------------------------------------------------------------------------------
void TClientMain::sl_Enter()
{
  bool ok;
  
  QByteArray baIP = ui.leIPServer->text().toLocal8Bit();
  char *sIP = (char*)baIP.data();

  unsigned int ip = ns_inet_addr(sIP);
  
  unsigned int port = ui.leLocalPort->text().toInt(&ok);
  if(ok==false) {Q_MESSAGE(tr("������������ ���� ���������� �����."));return;}
  
  QString nick = ui.leNik->text();
  if(nick.length()==0){Q_MESSAGE(tr("�� ����� ���"));return;}
  //-------------------------------
  // ��������� � ������ ��������� ���������
  SaveInReestr(ip,port,nick);
  //-------------------------------
  // ����������� ����
  QByteArray baNick = nick.toLocal8Bit();
  char* sNick = baNick.data();
  if(!pClient->Connect(ip,port,sNick))
		Q_MESSAGE(tr("��� ����� � ��������."));
}
//---------------------------------------------------------------------------------------------
void TClientMain::SaveInReestr(unsigned int ip, unsigned int port,QString& nick)
{
	QSettings settings("RUSSIA","ClientMain");
	settings.setValue("ip", ip);
	settings.setValue("port", port);
	settings.setValue("nick", nick);
}
//---------------------------------------------------------------------------------------------
void TClientMain::LoadFromReestr()
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
void TClientMain::Translate( unsigned short type,char *pData, int size )
{
  // ��������
}
//---------------------------------------------------------------------------------------------
void TClientMain::Connect(char* sNick, char* ip,char* port)// ��������� ������-����������
{
  ui.leNik->setText(tr(sNick));
  ui.leLocalPort->setText(tr(port));
  ui.leIPServer->setText(tr(ip));
  sl_Enter();
}
//---------------------------------------------------------------------------------------------
