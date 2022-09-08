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


#include "ManagerGUI.h"
#include "ClientTank.h"
#include <QMessageBox>
#include "ManagerDirectX.h"


class TManagerGUIEvent: public QEvent
{
  int mSize;
  char* pData;
  unsigned short typePacket;

public:
  TManagerGUIEvent(Type type):QEvent(type)
  {
    mSize = 0;
    pData = NULL;
  }
  ~TManagerGUIEvent()
  {
    mSize = 0;
    delete pData;
    pData = NULL;
  }
  void SetTypePacket(unsigned short val){typePacket=val;}
  unsigned short GetType(){return typePacket;}
  void SetData(char* data, int size)
  {
    pData = data;
    mSize = size;
  }
  char* GetData(int &size)
  {
    char* data = pData;
    size = mSize;
    
    mSize = 0;
    pData = NULL;
    return data;
  }
};
//-------------------------------------------------------------------------------------------
TManagerGUI* pManagerGUI = NULL;
//-------------------------------------------------------------------------------------------
void CallBackExitDirectX(unsigned char codeExit)
{
  if(pManagerGUI==NULL) {BL_FIX_BUG();return;}
  TManagerGUIEvent * event = new TManagerGUIEvent(QEVENT_DIRECTX_EXIT);
  QCoreApplication::postEvent( pManagerGUI, (QEvent*)event );
}
//-------------------------------------------------------------------------------------------
void CallBackPacketManagerGUI(void* data, int size)
{
  if(pManagerGUI==NULL) {BL_FIX_BUG();return;}
  unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));

  //---------------------------------------------------------------
  // фильтр
  switch(type)
  {
    case APPL_TYPE_G_A_CORRECT_PACKET_STATE_OBJECT:
    case APPL_TYPE_G_A_CORRECT_PACKET_STATE_TANK:
    case APPL_TYPE_G_A_SCORE:
    case APPL_TYPE_G_A_EVENT_IN_FIGHT:
      return;
  }
  //---------------------------------------------------------------
  char* dataPacket = (char*)data + sizeof(TIP_Port);
  int sizePacket   = size - sizeof(TIP_Port);

  TManagerGUIEvent * event = new TManagerGUIEvent(QEVENT_PACKET);
  char* pData = new char[sizePacket];
  memcpy(pData,dataPacket,sizePacket);
  event->SetData(pData,sizePacket);
  event->SetTypePacket(type);
  QCoreApplication::postEvent( pManagerGUI, event );
}
//-------------------------------------------------------------------------------------------
void CallBackStreamManagerGUI(void* data, int size)
{
  if(pManagerGUI==NULL) {BL_FIX_BUG();return;}
  unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));

  //---------------------------------------------------------------
  // фильтр
  switch(type)
  {
    case APPL_TYPE_G_S_LOAD_MAP:
    case APPL_TYPE_G_S_COUNT_DOWN:
    case APPL_TYPE_G_S_FIGHT_COORD_BULLET:
      // обработка возложена на буфферизатор
      return;
  }
  //---------------------------------------------------------------

  char* dataPacket = (char*)data + sizeof(TIP_Port);
  int sizePacket   = size - sizeof(TIP_Port);

  TManagerGUIEvent * event = new TManagerGUIEvent(QEVENT_STREAM);
  char* pData = new char[sizePacket];
  memcpy(pData,dataPacket,sizePacket);
  event->SetData(pData,sizePacket);
  event->SetTypePacket(type);
  QCoreApplication::postEvent( pManagerGUI, event );
}
//-------------------------------------------------------------------------------------------
void CallBackDisconnectManagerGUI(void* data, int size)// оставить
{
  if(pManagerGUI==NULL) {BL_FIX_BUG();return;}
  TManagerGUIEvent * event = new TManagerGUIEvent(QEVENT_DISCONNECT);
  QCoreApplication::postEvent( pManagerGUI, event );
}
//-------------------------------------------------------------------------------------------

TManagerGUI::TManagerGUI(QWidget* parent):QObject(parent)
{
  pManagerGUI = this;

  GlobalClientTank.Register(CallBackPacketManagerGUI,nsCallBackType::eRcvPacket);
  GlobalClientTank.Register(CallBackStreamManagerGUI,nsCallBackType::eRcvStream);
  GlobalClientTank.Register(CallBackDisconnectManagerGUI,nsCallBackType::eDisconnect);

  pCurrentForm =&mClientMain;
}
//-----------------------------------------------------------------------
TManagerGUI::~TManagerGUI()
{
  GlobalClientTank.Unregister(CallBackPacketManagerGUI,nsCallBackType::eRcvPacket);
  GlobalClientTank.Unregister(CallBackStreamManagerGUI,nsCallBackType::eRcvStream);
  GlobalClientTank.Unregister(CallBackDisconnectManagerGUI,nsCallBackType::eDisconnect);
  
  pManagerGUI = NULL;
  Done();
}
//-----------------------------------------------------------------------
void TManagerGUI::start()
{
  pManagerDirectX = new TManagerDirectX;
  pManagerDirectX->Start(CallBackExitDirectX);
  pManagerDirectX->SetKeyHandler(&mKeyHandler);
  pManagerDirectX->SetMouseHandler(&mMouseHandler);
  mKeyHandler.SetManagerDirectX(pManagerDirectX);

  mGameForm.SetManagerDirectX(pManagerDirectX);// дать информацию о ManagerDirectX

  if(pCurrentForm)
    pCurrentForm->showGUI();
}
//-----------------------------------------------------------------------
void TManagerGUI::stop()
{
  Done();
  _exit(0);
}
//-----------------------------------------------------------------------
void TManagerGUI::Connect(char* sNick, char* ip,char* port)// поддержка скрипт-управления
{
  // кривая реализация, потом исправлю
  mClientMain.Connect( sNick, ip, port);
}
//-----------------------------------------------------------------------
void TManagerGUI::customEvent( QEvent * e  )
{
  TManagerGUIEvent* event = (TManagerGUIEvent*)e;
  switch(event->type())
  {
    case QEVENT_DISCONNECT:
      Q_MESSAGE(tr("Разрыв связи с сервером."))
      // создать форму входа в сервер
      OpenClientMainForm();
      break;
    case QEVENT_PACKET:
      WorkPacket(event);    
      break;
    case QEVENT_STREAM:
      WorkStream(event);
      break;
    case QEVENT_DIRECTX_EXIT:
      stop();
      break;
    default:;
  }
}
//---------------------------------------------------------------------------------------------
void TManagerGUI::WorkStream(TManagerGUIEvent* event)
{
  int size;
  char* pData = event->GetData(size);
  unsigned short type = event->GetType();
  switch(type)
  {
    default:
    {  
      if(pCurrentForm)
        pCurrentForm->Translate(type,pData,size);
    };
  }
  delete pData;
}
//---------------------------------------------------------------------------------------------
void TManagerGUI::WorkPacket(TManagerGUIEvent* event)
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
void TManagerGUI::AnswerFromServer_Enter(unsigned char mCodeAnswer)
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
void TManagerGUI::AnalizCode_A_In_Fight(char* pData, int size)
{
  TA_In_Fight packet;
  packet.setData(pData,size);

  switch(packet.getCode())
  {
    case TA_In_Fight::eFight:
      OpenGameForm();
      // загрузка карты по коду
      pManagerDirectX->LoadMap(packet);
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
void TManagerGUI::OpenWaitForm()
{
  // создать форму ожидания
  pCurrentForm->hideGUI();
  pCurrentForm = &mWaitForm;
  pCurrentForm->showGUI();
}
//---------------------------------------------------------------------------------------------
void TManagerGUI::OpenGameForm()
{
  // создать форму боя
  pCurrentForm->hideGUI();
  pCurrentForm = &mGameForm;
  pCurrentForm->showGUI();
}
//---------------------------------------------------------------------------------------------
void TManagerGUI::OpenRoomForm()
{
  // создать форму гаража
  pCurrentForm->hideGUI();
  pCurrentForm = &mGameRoomPrepare;
  pCurrentForm->showGUI();
}
//---------------------------------------------------------------------------------------------
void TManagerGUI::OpenClientMainForm()
{
  // в главное окно
  pCurrentForm->hideGUI();
  pCurrentForm = &mClientMain;
  pCurrentForm->showGUI();
}
//---------------------------------------------------------------------------------------------
void TManagerGUI::Analiz_End_Fight(char* pData, int size)
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
void TManagerGUI::ExitFromFight()
{
  OpenRoomForm();
}
//---------------------------------------------------------------------------------------------
void TManagerGUI::ExitFromWait()
{
  OpenRoomForm();
}
//---------------------------------------------------------------------------------------------
void TManagerGUI::Done()
{
  delete pManagerDirectX;
  pManagerDirectX = NULL;
}
//---------------------------------------------------------------------------------------------
