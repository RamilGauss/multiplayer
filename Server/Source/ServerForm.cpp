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


#include "ServerForm.h"
#include <QTimer>
#include <QTableWidgetItem>
#include "ServerStruct.h"
#include "NetSystem.h"

ServerForm* pServerForm = NULL;

void CallBackEventServerForm(void* data, int size)
{
  if(pServerForm==NULL) {BL_FIX_BUG();return;}
  pServerForm->setArrClient(data,size);

  QEvent * event = new QEvent(QEvent::User);
  QCoreApplication::postEvent( pServerForm, event );
}
//-------------------------------------------------------------------------------------------
ServerForm::ServerForm(QWidget *parent)
: QWidget(parent),mServer(SERVER_NUMBER_OF_NETWORK)
{
	mServer.registerRequest(CallBackEventServerForm);

	pArr = NULL;

	pServerForm = this;

  ui.setupUi(this);

  setWindowTitle(tr(STR_VERSION_SERVER));

  // ������ ������� ��� ���������� ������ ��������
  QTimer * timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(sl_RequestArrClient()));
	timer->start(eTime);
  ui.tableClient->clear();
  ui.tableClient->setRowCount(0);

  ui.tableRoom->clear();
  ui.tableRoom->setRowCount(0);


  mServer.start();
	RefreshTable();
}
//---------------------------------------------------------------------------------------------
ServerForm::~ServerForm()
{
  mServer.stop();
	Done();
}
//---------------------------------------------------------------------------------------------
void ServerForm::customEvent( QEvent * event  )
{
	switch(event->type())
	{
		case QEvent::User:
			RefreshTable();
		default:;
	}
}
//---------------------------------------------------------------------------------------------
void ServerForm::RefreshTable()
{
  if(pArr)
  {
    ui.tableClient->setRowCount(pArr->cntClient);
    ui.tableRoom->setRowCount(pArr->cntRoom);
  }
  else
  {
    ui.tableClient->setRowCount(0);
    ui.tableRoom->setRowCount(0);
  }

	if(pArr==NULL) return;
  
  RefreshTableClient();
  RefreshTableRoom();
}
//---------------------------------------------------------------------------------------------
void ServerForm::setArrClient(void* data,int size)
{
	Done();
	if(data)
		pArr = (ServerTank::TAnswerRequest*)data;
}
//---------------------------------------------------------------------------------------------
void ServerForm::Done()
{
	delete pArr;
	pArr = NULL;
}
//---------------------------------------------------------------------------------------------
void ServerForm::sl_RequestArrClient()
{
	mServer.requestListClient();
}
//---------------------------------------------------------------------------------------------
void ServerForm::RefreshTableClient()
{
  ServerTank::TDefClient * pArrClient = pArr->mArrClient;
  int cntClient                       = pArr->cntClient;

  for(int i = 0 ; i < cntClient ; i++)
  {
    // ��� 
    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText(tr(pArrClient[i].name));
    if(pArrClient[i].flgDisconnect)
      item->setForeground(QBrush(QColor(0xff,0,0)));
    else
      item->setForeground(QBrush(QColor(0,0xff,0)));

    ui.tableClient->setItem(i, 0, item);

    // ���������
    QString sState;
    switch(pArrClient[i].state)
    {
      case TClient::eGarage:
        sState = tr("�����");
        break;
      case TClient::eWait:
        sState = tr("����");
        break;
      case TClient::eFight:
        sState = tr("���");
        break;
      default:BL_FIX_BUG();;
    }
    item = new QTableWidgetItem();
    item->setText(sState); 
    ui.tableClient->setItem(i, 1, item);
    // ip:port
    char sIP_Port[100];
    sprintf(sIP_Port,"%s:%u",ns_str_addr(pArrClient[i].ip),pArrClient[i].port);
    QString strIP_Port = tr(sIP_Port);
    item = new QTableWidgetItem();
    item->setText(strIP_Port); 
    ui.tableClient->setItem(i, 2, item);
  }
  ui.tableClient->resizeColumnsToContents();
}
//-------------------------------------------------------------------------------------------
void ServerForm::RefreshTableRoom()
{
  ServerTank::TDefRoom * pArrRoom = pArr->mArrRoom;
  int cntRoom                     = pArr->cntRoom;

  for(int i = 0 ; i < cntRoom ; i++)
  {
    // ��� 
    QTableWidgetItem* item = new QTableWidgetItem;
    char s_time_rest[100];
    sprintf(s_time_rest,"%02u:%02u",pArrRoom[i].time_rest/60,pArrRoom[i].time_rest%60);
    item->setText(tr(s_time_rest));
    ui.tableRoom->setItem(i, 0, item);
    //-------------------------------------------------------
    item = new QTableWidgetItem();
    char s_active[100];
    sprintf(s_active,"%d",pArrRoom[i].cntActiveClient);
    item->setText(tr(s_active)); 
    ui.tableRoom->setItem(i, 1, item);
  }
  ui.tableRoom->resizeColumnsToContents();
}
//-------------------------------------------------------------------------------------------