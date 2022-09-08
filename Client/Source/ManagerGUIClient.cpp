/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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


#include "ManagerGUIClient.h"
#include <QCoreApplication.h>
#include <QMessageBox>
#include "TransportProtocolPacket.h"
#include "ClientTank.h"


TManagerGUIClient* pManagerGUIClient = NULL;
//-------------------------------------------------------------------------------------------
void CallBackPacketManagerGUI(void* data, int size)
{
  if(pManagerGUIClient==NULL) {BL_FIX_BUG();return;}
  unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));
  //---------------------------------------------------------------
  char* dataPacket = (char*)data + sizeof(TIP_Port);
  int sizePacket   = size - sizeof(TIP_Port);
  //---------------------------------------------------------------
  TManagerGUIEvent * event = new TManagerGUIEvent(QEVENT_PACKET);
  char* pData = new char[sizePacket];
  memcpy(pData,dataPacket,sizePacket);
  event->SetData(pData,sizePacket);
  event->SetTypePacket(type);
  QCoreApplication::postEvent( pManagerGUIClient, event );
}
//-------------------------------------------------------------------------------------------
void CallBackStreamManagerGUI(void* data, int size)
{
  if(pManagerGUIClient==NULL) {BL_FIX_BUG();return;}
  unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));
  //---------------------------------------------------------------
  char* dataPacket = (char*)data + sizeof(TIP_Port);
  int sizePacket   = size - sizeof(TIP_Port);
  //---------------------------------------------------------------
  TManagerGUIEvent * event = new TManagerGUIEvent(QEVENT_STREAM);
  char* pData = new char[sizePacket];
  memcpy(pData,dataPacket,sizePacket);
  event->SetData(pData,sizePacket);
  event->SetTypePacket(type);
  QCoreApplication::postEvent( pManagerGUIClient, event );
}
//-------------------------------------------------------------------------------------------
void CallBackDisconnectManagerGUI(void* data, int size)// оставить
{
  if(pManagerGUIClient==NULL) {BL_FIX_BUG();return;}
  TManagerGUIEvent * event = new TManagerGUIEvent(QEVENT_DISCONNECT);
  QCoreApplication::postEvent( pManagerGUIClient, event );
}
//-------------------------------------------------------------------------------------------
  //-----------------------------------------------------------------------------------------
TManagerGUIClient::TManagerGUIClient(QWidget* parent):TManagerGUI(parent)
{
  pManagerGUIClient = this;
}
//-----------------------------------------------------------------------
TManagerGUIClient::~TManagerGUIClient()
{
  pManagerGUIClient = NULL;
}
//-----------------------------------------------------------------------
void TManagerGUIClient::WorkPacket(TManagerGUIEvent* event)
{
  int size;
  char* pData = event->GetData(size);
  unsigned short type = event->GetType();
  switch(type)
  {
    case APPL_TYPE_A_TRY_CONNECT_TO_SERVER:
    {
      TA_Try_Connect_To_Server packet;
      packet.setData(pData,size);
      AnswerFromServer_Enter(packet.getCode());
      break;
    }
    case APPL_TYPE_A_IN_FIGHT:
    {
      AnalizCode_A_In_Fight(pData,size);
      break;
    }
    case APPL_TYPE_A_END_FIGHT:
    {
      Analiz_End_Fight(pData,size);
      break;
    }
    case APPL_TYPE_A_EXIT_WAIT:
    {
      ExitFromWait();
      break;
    }
    case APPL_TYPE_A_EXIT_FIGHT:
    {
      ExitFromFight();
      break;
    }
    default:
    {  
      if(pCurrentForm)
        pCurrentForm->Translate(type,pData,size);
    };
  }
  delete pData;
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::startEvent()
{
  pClient->Register(CallBackPacketManagerGUI,nsCallBackType::eRcvPacket);
  pClient->Register(CallBackStreamManagerGUI,nsCallBackType::eRcvStream);
  pClient->Register(CallBackDisconnectManagerGUI,nsCallBackType::eDisconnect);
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::stopEvent()
{
  pClient->Unregister(CallBackPacketManagerGUI,nsCallBackType::eRcvPacket);
  pClient->Unregister(CallBackStreamManagerGUI,nsCallBackType::eRcvStream);
  pClient->Unregister(CallBackDisconnectManagerGUI,nsCallBackType::eDisconnect);
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::AnswerFromServer_Enter(unsigned char mCodeAnswer)
{
  QString sError;
  // удачно
  switch(mCodeAnswer)
  {
    case TA_Try_Connect_To_Server::eConnectGarage:
      OpenRoomForm();
      break;
    case TA_Try_Connect_To_Server::eConnectWait:
      OpenWaitForm();       
      break;
    case TA_Try_Connect_To_Server::eConnectFight:
      BL_FIX_BUG();
      //OpenGameForm();
      break;
    case TA_Try_Connect_To_Server::eOldVersion:
      sError = tr("Обновите клиент.Старая версия.");
      break;
    case TA_Try_Connect_To_Server::eWasConnect:
      sError = tr("Повторная попытка соединения.");
      break;
    case TA_Try_Connect_To_Server::eBlackList:
      sError = tr("Клиент в черном списке.");
      break;
    case TA_Try_Connect_To_Server::eOverloadServer:
      sError = tr("Сервер перегружен. Попытайтесь зайти попозже.");
      break;
    default:BL_FIX_BUG();
  }
  if(sError.length())
    Q_MESSAGE(sError);
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::AnalizCode_A_In_Fight(char* pData, int size)
{
  TA_In_Fight packet;
  packet.setData(pData,size);

  switch(packet.getCode())
  {
    case TA_In_Fight::eFight:
      OpenGameForm();
      // загрузка карты по коду
      pCurrentForm->Translate(APPL_TYPE_A_IN_FIGHT,pData, size);
      break;
    case TA_In_Fight::eWait:
      OpenWaitForm();
      break;
    case TA_In_Fight::eFailBlockTank:
      Q_MESSAGE(tr("Танк заблокирован. Выберете другой."))
        break;
    default:BL_FIX_BUG();
  }
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::OpenWaitForm()
{
  // создать форму ожидания
  OpenForm("waitForm");
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::OpenGameForm()
{
  // создать форму боя
  OpenForm("gameForm");
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::OpenRoomForm()
{
  // создать форму гаража
  OpenForm("gameRoomPrepare");
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::OpenClientMainForm()
{
  // в главное окно
  OpenForm("clientMain");
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::Analiz_End_Fight(char* pData, int size)
{
  TA_End_Fight A_End_Fight;
  A_End_Fight.setData(pData,size);

  if(A_End_Fight.getCodeExit()==TA_End_Fight::eExitTrue)
  {
    OpenRoomForm();
    char str[400];
    A_End_Fight.getMsg(str);
    Q_MESSAGE(tr(str));
  }
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::ExitFromFight()
{
  OpenRoomForm();
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::ExitFromWait()
{
  OpenRoomForm();
}
//---------------------------------------------------------------------------------------------
void TManagerGUIClient::OpenFirstForm()
{
  OpenClientMainForm();
}
//---------------------------------------------------------------------------------------------
