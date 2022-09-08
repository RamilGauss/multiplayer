/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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


#include "Room.h"
#include "ApplicationProtocolPacketStream.h"
#include "Tank.h"
#include "ApplicationProtocolPacketAnswer.h"
#include "ApplicationProtocolMainPacket.h"
#include "BL_Debug.h"

using namespace nsServerStruct;


TRoom::TRoom()
{
  mTransport          = NULL;
  mTimeBeginCountDown = guint32(-1);
  mTimeAfterCountDown = guint32(-1);
  mState              = eLoad;
  flgEventReconnect   = false;
  flgIsLoadingMap     = false;
  mID_map             = 0;
}
//----------------------------------------------------------------
TRoom::~TRoom()
{
  mArrTank.Unlink();
}
//----------------------------------------------------------------
void TRoom::AddTank(TTank* pTank)
{
  mArrTank.Add(pTank);
  // подготовка к бою
  pTank->mSpeed = 0;
  pTank->mMaskPushButton = 0;// ничего не нажато
}
//----------------------------------------------------------------
void TRoom::SetTransport(TransportProtocolTank* pTransport)
{
  mTransport = pTransport;
}
//----------------------------------------------------------------
bool TRoom::Work()
{
  mNow_MS = ht_GetMSCount();

  // что прислали клиенты, раскидать по спискам
  AnalizPacket();// здесь отослать корректирующий пакет (можно парти€ми)
  //-------------------------------------------------------------
  // поместить в ѕредсказатель инфо и событи€х из списка событий по объектам
  if(mState==eFight)
  {
    // расчет данных
    CalcPrediction();
  }
  //-------------------------------------------------------------
  // –ассылка Stream - обратный отсчет, координаты и событи€х бо€ (как результат ѕредсказател€)
  // если только началс€ - загрузить карту
  WorkByState();
  //-------------------------------------------------------------
  // проверить кончилс€ ли бой
  if(CheckEndFight()) 
    return false;
  
  return true;
}
//----------------------------------------------------------------
void TRoom::WriteTransportStream(TClient* pClient,TBasePacket *packet)
{
  TransportProtocolTank::InfoData* infoData = new TransportProtocolTank::InfoData;
  infoData->ip_dst   = pClient->ip;
  infoData->port_dst = pClient->port;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();
  mTransport->write(infoData,false);     
}
//----------------------------------------------------------------------------------
void TRoom::WriteTransportAnswer(TClient* pClient,TBasePacket *packet)
{
  TransportProtocolTank::InfoData* infoData = new TransportProtocolTank::InfoData;
  infoData->ip_dst   = pClient->ip;
  infoData->port_dst = pClient->port;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();
  mTransport->write(infoData);     
}
//----------------------------------------------------------------------------------
void TRoom::Done()
{
  // разослать результаты бо€
  for(int i = 0 ; i < mArrTank.Count() ; i++)
  {
    TTank* pTank = (TTank*)mArrTank.Get(i);

    TClient* pClient = pTank->GetMasterClient();
    if(pClient->GetCurRoom()==pTank->pRoom)// а вдруг клиент на другом танке в другом бою?
    {
      pClient->state = TClient::eGarage;
      pClient->SetCurRoom(NULL);
    }

    pTank->pRoom = NULL;// отв€зать от комнаты
    //--------------------------------------------------------------------
    // отсылать только если есть св€зь с клиентом
    if(pClient->flgDisconnect==false)
    {
      // отослать результаты бо€
      TA_End_Fight A_End_Fight;
      A_End_Fight.setCodeAction(TA_End_Fight::eNoOnes);// ### ничь€

      // а вдруг клиент на другом танке в другом бою или в ожидании?
      if((pClient->GetCurRoom()==pTank->pRoom||   // клиент находитс€ в бою в этой комнате
        pClient->GetCurRoom()==NULL          )&&// клиент вышел из комнаты и ждет окончани€ бо€
        pClient->state!=TClient::eWait       )  // не в ожидании
        A_End_Fight.setCodeExit(TA_End_Fight::eExitTrue);// ManagerGui закроет окно DX и откроет окно комнаты подготовке
      else
        A_End_Fight.setCodeExit(TA_End_Fight::eExitFalse);// идет бой на другом танке

      char strMsg[100];
      sprintf(strMsg, "ѕриветик. Ѕой то закончилс€. Ќичь€.¬рем€: %u сек.", (mNow_MS-mTimeAfterCountDown)/1000);
      A_End_Fight.setMsg(strMsg);

      WriteTransportAnswer(pClient,&A_End_Fight);
    }
  }
  mArrTank.Unlink();
}
//----------------------------------------------------------------------------------
void TRoom::WorkByState()
{
  // обратный отсчет
  switch(mState)
  {
    case eLoad:
      WorkLoadMap();
      break;
    case eCountDown:
      WorkCountDown();
      break;
    case eFight:
      WorkFight();
      break;
    default:BL_FIX_BUG();;
  }
}
//----------------------------------------------------------------------------------
bool TRoom::CheckEndFight()
{
  if(mState==eFight)
  if(mNow_MS>mTimeAfterCountDown+DURATION_FIGHT_MS)
      return true;
  // вставить проверку флага окончани€ бо€ по причине захвата базы или уничтожени€ всех танков одной из команд
  // или же ничью

  return false;
}
//----------------------------------------------------------------------------------
void TRoom::WorkCountDown()
{
  if(mNow_MS<mTimeBeginCountDown+eTimeCountDown)
  {
    TS_Count_Down S_Count_Down;
    float second_rest = float(mTimeBeginCountDown+eTimeCountDown-mNow_MS)/1000;
    S_Count_Down.setWait(second_rest);
    int cnt = mArrTank.Count();
    for(int i = 0 ; i < cnt ; i++)
    {
      TTank* pTank = (TTank*)mArrTank.Get(i);
      TClient* pClient = pTank->GetMasterClient();
      WriteTransportStream(pClient,&S_Count_Down);
    }
  }
  else
  {
    mState = eFight;
    mTimeAfterCountDown = mNow_MS;
  }
}
//----------------------------------------------------------------------------------
void TRoom::WorkFight()
{
  int cnt = mArrTank.Count();

  TS_Fight_Coord_Bullet s_coord;
  s_coord.setCntTank(cnt);
  for(int i = 0 ; i < cnt ; i++)
  {
    TTank* pTank = (TTank*)mArrTank.Get(i);
    TClient* pClient = pTank->GetMasterClient();
    if(pClient->GetCurRoom()==this)// уменьшить трафик
      WriteTransportStream(pClient,&s_coord);
  }
  //------------------------------------------------------------------------------------
  if(mPrediction.mListFreshEvent.size())
  {
    std::list<TPrediction::TEvent*>::iterator it = mPrediction.mListFreshEvent.begin();
    std::list<TPrediction::TEvent*>::iterator eit = mPrediction.mListFreshEvent.end();
    TA_Event_In_Fight event_packet;
    event_packet.setCnt(mPrediction.mListFreshEvent.size());
    while(it!=eit)// формирование пакета
    {
      //event_packet.setID((*it)->id);
      //event_packet.setID((*it)->state);
      it++;
    }
    //------------------------------------------------------------------------------------
    for(int i = 0 ; i < cnt ; i++)
    {
      TTank* pTank = (TTank*)mArrTank.Get(i);
      TClient* pClient = pTank->GetMasterClient();
      if(pClient->GetCurRoom()==this)// уменьшить трафик
        WriteTransportAnswer(pClient,&event_packet);
    }
  }
}
//----------------------------------------------------------------------------------
void TRoom::SetIDMap(unsigned short val)
{
  mID_map = val;
}
//----------------------------------------------------------------------------------
bool TRoom::CheckIsLoadComplete()
{
  return flgIsLoadingMap;
}
//----------------------------------------------------------------------------------
void TRoom::MakeGroup()
{
  int cnt = mArrTank.Count();
  for(int i = 0 ; i < cnt ; i++)
  {
    TTank* pTank = (TTank*)mArrTank.Get(i);
    if(i>cnt/2)
      pTank->mGroup = 1;
    else
      pTank->mGroup = 0;
  }
}
//----------------------------------------------------------------------------------
void TRoom::PreparePrediction()
{
  mPrediction.InitState();
}
//----------------------------------------------------------------------------------
void TRoom::WorkLoadMap()
{
  if(CheckIsLoadComplete())
  {
    mState = eCountDown;
    mTimeBeginCountDown = mNow_MS;
  }
}
//----------------------------------------------------------------------------------
void TRoom::AnalizPacket()// здесь отослать корректирующий пакет (можно парти€ми)
{
  std::list<TAction*>::iterator it  = mListFreshAction.begin();
  std::list<TAction*>::iterator eit = mListFreshAction.end();
  while(it!=eit)
  {
    unsigned short type = (*it)->pDefPacket->packet->getType();
    TClient* pClient = (*it)->pTank->GetMasterClient();
    switch(type)
    {
      case APPL_TYPE_R_CORRECT_PACKET:
      {
        // если не бой, то отослать координаты расставленных танков
        if(mState!=eFight)
          SendInitCoordTank(pClient);
        // танк в бою, надо дать инфу по счету и времени
        if(mState==eFight)
          SendScore(pClient);
        
        SendStateObject(pClient);
        break;
      }
      case APPL_TYPE_S_ORIENT_AIM:
      {
        BL_ASSERT(mState!=eFight);
        // добавить в prediction
        mPrediction.SetOrientAim((*it)->pTank,(*it)->pDefPacket);
        break;
      }
      case APPL_TYPE_C_KEY_EVENT:
      {
        BL_ASSERT(mState!=eFight);
        // добавить в prediction
        mPrediction.SetKeyEvent((*it)->pTank,(*it)->pDefPacket);
        break;
      }
      default:BL_FIX_BUG();
    }
    delete *it;
    it++;
  }
  mListFreshAction.clear();
}
//----------------------------------------------------------------------------------
// расчет данных
void TRoom::CalcPrediction()
{
  mPrediction.Calc();
}
//----------------------------------------------------------------------------------
void* ThreadLoadMap(void* p)
{
  ((TRoom*)(p))->ThreadLoadMap();
  return NULL;
}
//----------------------------------------------------------------------------------
void TRoom::ThreadLoadMap()
{
  // загрузка
  //mID_map
  ht_msleep(5000);// симул€ци€
  flgIsLoadingMap = false;
  PreparePrediction();
}
//----------------------------------------------------------------------------------
void TRoom::LoadMap()
{
  // при условии что список танков уже есть
  BL_ASSERT(mArrTank.Count());
  InitPositionTank();

  flgIsLoadingMap = true;
  threadLoadMap = g_thread_create(::ThreadLoadMap, (gpointer)this, true, NULL);
}
//----------------------------------------------------------------------------------
void TRoom::SetPacket(nsServerStruct::TPacketServer* pDefPacket,TTank* pTank)
{
  TAction* pAction = new TAction;
  pAction->pTank   = pTank;
  pAction->pDefPacket = pDefPacket;
  mListFreshAction.push_back(pAction);
}
//----------------------------------------------------------------------------------
void TRoom::SendInitCoordTank(TClient* pClient)
{
  TA_Correct_Packet_State_Tank packet;
  // расчет координат возможен до загрузки карты, все данные хран€тс€ у предсказател€
  int cnt = mArrTank.Count();
  packet.setCountTank(cnt);
  for(int i = 0 ; i < cnt ; i++)
  {
    TTank* pTank = (TTank*)mArrTank.Get(i);
    packet.setID(i,i);
    packet.setX(i,pTank->mCoord.x);
    packet.setY(i,pTank->mCoord.y);  
    packet.setZ(i,pTank->mCoord.z);
    packet.setXV(i,pTank->mOrient.vx);
    packet.setYV(i,pTank->mOrient.vy);
    packet.setZV(i,pTank->mOrient.vz);
  }
  WriteTransportAnswer(pClient,&packet);
}
//----------------------------------------------------------------------------------
void TRoom::SendScore(TClient* pClient)
{
  TA_Score A_Score;
  A_Score.setScore0(score0);
  A_Score.setScore1(score1);
  A_Score.setTimeRest(DURATION_FIGHT_MS - (mNow_MS - mTimeAfterCountDown));
  WriteTransportAnswer(pClient,&A_Score);
}
//----------------------------------------------------------------------------------
void TRoom::SendStateObject(TClient* pClient)
{
  TA_Correct_Packet_State_Object packet;
  // все данные хран€тс€ у предсказател€
  std::list<TObjectPrediction*>::iterator it  = mPrediction.mListDamageObject.begin();
  std::list<TObjectPrediction*>::iterator eit = mPrediction.mListDamageObject.end();

  int cnt = mPrediction.mListDamageObject.size();
  if(cnt==0) return;//###
  //---------------------------------------------------------
  packet.setCountObject(cnt);
  int i = 0;
  while(it!=eit)
  {
    packet.setID(i,(*it)->GetID());
    packet.setMaskState(i,(*it)->GetMaskState());
    it++;
    i++;
  }
  // если объектов очень много, то разбить на группы
  WriteTransportAnswer(pClient,&packet);
}
//----------------------------------------------------------------------------------
void TRoom::InitPositionTank()
{
  mID_map;
}
//----------------------------------------------------------------------------------
// отладка
int TRoom::GetTimeRest_sec()
{
  if(mState==eFight)
    return DURATION_FIGHT_MS/1000 - (mNow_MS-mTimeAfterCountDown)/1000;
  return 0;
}
//----------------------------------------------------------------------------------
int TRoom::GetActiveClient()
{
  int cntActive = 0;
  int cnt = mArrTank.Count();
  for(int i = 0 ; i < cnt ; i++ )
  {
    TTank* pTank = (TTank*)mArrTank.Get(i);
    if(pTank)
    if(pTank->GetMasterClient()->GetCurRoom()==this)
      cntActive++;
  }
  return cntActive;
}
//----------------------------------------------------------------------------------