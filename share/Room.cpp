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
  // ���������� � ���
  pTank->mSpeed = 0;
  pTank->mMaskPushButton = 0;// ������ �� ������
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

  // ��� �������� �������, ��������� �� �������
  AnalizPacket();// ����� �������� �������������� ����� (����� ��������)
  //-------------------------------------------------------------
  // ��������� � ������������� ���� � �������� �� ������ ������� �� ��������
  if(mState==eFight)
  {
    // ������ ������
    CalcPrediction();
  }
  //-------------------------------------------------------------
  // �������� Stream - �������� ������, ���������� � �������� ��� (��� ��������� �������������)
  // ���� ������ ������� - ��������� �����
  WorkByState();
  //-------------------------------------------------------------
  // ��������� �������� �� ���
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
  // ��������� ���������� ���
  for(int i = 0 ; i < mArrTank.Count() ; i++)
  {
    TTank* pTank = (TTank*)mArrTank.Get(i);

    TClient* pClient = pTank->GetMasterClient();
    if(pClient->GetCurRoom()==pTank->pRoom)// � ����� ������ �� ������ ����� � ������ ���?
    {
      pClient->state = TClient::eGarage;
      pClient->SetCurRoom(NULL);
    }

    pTank->pRoom = NULL;// �������� �� �������
    //--------------------------------------------------------------------
    // �������� ������ ���� ���� ����� � ��������
    if(pClient->flgDisconnect==false)
    {
      // �������� ���������� ���
      TA_End_Fight A_End_Fight;
      A_End_Fight.setCodeAction(TA_End_Fight::eNoOnes);// ### �����

      // � ����� ������ �� ������ ����� � ������ ��� ��� � ��������?
      if((pClient->GetCurRoom()==pTank->pRoom||   // ������ ��������� � ��� � ���� �������
        pClient->GetCurRoom()==NULL          )&&// ������ ����� �� ������� � ���� ��������� ���
        pClient->state!=TClient::eWait       )  // �� � ��������
        A_End_Fight.setCodeExit(TA_End_Fight::eExitTrue);// ManagerGui ������� ���� DX � ������� ���� ������� ����������
      else
        A_End_Fight.setCodeExit(TA_End_Fight::eExitFalse);// ���� ��� �� ������ �����

      char strMsg[100];
      sprintf(strMsg, "��������. ��� �� ����������. �����.�����: %u ���.", (mNow_MS-mTimeAfterCountDown)/1000);
      A_End_Fight.setMsg(strMsg);

      WriteTransportAnswer(pClient,&A_End_Fight);
    }
  }
  mArrTank.Unlink();
}
//----------------------------------------------------------------------------------
void TRoom::WorkByState()
{
  // �������� ������
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
  // �������� �������� ����� ��������� ��� �� ������� ������� ���� ��� ����������� ���� ������ ����� �� ������
  // ��� �� �����

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
    if(pClient->GetCurRoom()==this)// ��������� ������
      WriteTransportStream(pClient,&s_coord);
  }
  //------------------------------------------------------------------------------------
  if(mPrediction.mListFreshEvent.size())
  {
    std::list<TPrediction::TEvent*>::iterator it = mPrediction.mListFreshEvent.begin();
    std::list<TPrediction::TEvent*>::iterator eit = mPrediction.mListFreshEvent.end();
    TA_Event_In_Fight event_packet;
    event_packet.setCnt(mPrediction.mListFreshEvent.size());
    while(it!=eit)// ������������ ������
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
      if(pClient->GetCurRoom()==this)// ��������� ������
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
void TRoom::AnalizPacket()// ����� �������� �������������� ����� (����� ��������)
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
        // ���� �� ���, �� �������� ���������� ������������� ������
        if(mState!=eFight)
          SendInitCoordTank(pClient);
        // ���� � ���, ���� ���� ���� �� ����� � �������
        if(mState==eFight)
          SendScore(pClient);
        
        SendStateObject(pClient);
        break;
      }
      case APPL_TYPE_S_ORIENT_AIM:
      {
        BL_ASSERT(mState!=eFight);
        // �������� � prediction
        mPrediction.SetOrientAim((*it)->pTank,(*it)->pDefPacket);
        break;
      }
      case APPL_TYPE_C_KEY_EVENT:
      {
        BL_ASSERT(mState!=eFight);
        // �������� � prediction
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
// ������ ������
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
  // ��������
  //mID_map
  ht_msleep(5000);// ���������
  flgIsLoadingMap = false;
  PreparePrediction();
}
//----------------------------------------------------------------------------------
void TRoom::LoadMap()
{
  // ��� ������� ��� ������ ������ ��� ����
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
  // ������ ��������� �������� �� �������� �����, ��� ������ �������� � �������������
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
  // ��� ������ �������� � �������������
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
  // ���� �������� ����� �����, �� ������� �� ������
  WriteTransportAnswer(pClient,&packet);
}
//----------------------------------------------------------------------------------
void TRoom::InitPositionTank()
{
  mID_map;
}
//----------------------------------------------------------------------------------
// �������
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