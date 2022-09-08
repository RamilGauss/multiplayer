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
  mTimeBeginCountDown  = guint32(-1);
  mTimeAfterCountDown = guint32(-1);
  mState              = eLoad;
  flgEventReconnect   = false;
  flgIsLoadingMap     = false;
  mID_map             = 0;
}
//----------------------------------------------------------------
TRoom::~TRoom()
{
  Done();
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
  {
    SendResultFight();
    return false;
  }
  
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
  for(int i = 0 ; i < mArrTank.Count() ; i++)
  {
    TTank* pTank = (TTank*)mArrTank.Get(i);
    pTank->pRoom = NULL;// �������� �� �������
    TClient* pClient = pTank->GetMasterClient();
    pClient->state = TClient::eGarage;
    
    TA_End_Fight A_End_Fight;
    WriteTransportAnswer(pClient,&A_End_Fight);
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
  {
    // �������� ������ �� ��������� ������� ���
    int cnt = mArrTank.Count();
    for(int i = 0 ; i < cnt ; i++)
    {
      TTank* pTank = (TTank*)mArrTank.Get(i);
      TClient* pClient = pTank->GetMasterClient();

      TA_End_Fight A_End_Fight;
      A_End_Fight.setCode(TA_End_Fight::eNoOnes); // �����
      WriteTransportAnswer(pClient,&A_End_Fight);
      return true;
    }
  }

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
  //s_coord.setArrBullet();// ����� ������ �� ������������� � ������� ����

  for(int i = 0 ; i < cnt ; i++)
  {
    TTank* pTank = (TTank*)mArrTank.Get(i);
    TClient* pClient = pTank->GetMasterClient();
    WriteTransportStream(pClient,&s_coord);
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
void TRoom::SendResultFight()
{
  int cnt = mArrTank.Count();
  for(int i = 0 ; i < cnt ; i++)
  {
    TTank* pTank = (TTank*)mArrTank.Get(i);
    TClient* pClient = pTank->GetMasterClient();

    TA_End_Fight A_End_Fight;
    // ��������� �����������
    A_End_Fight.setCode(TA_End_Fight::eWin);// ���� ������

    WriteTransportAnswer(pClient,&A_End_Fight);
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
        mPrediction.SetOrientAim(pClient);
        break;
      }
      case APPL_TYPE_C_KEY_EVENT:
      {
        BL_ASSERT(mState!=eFight);
        // �������� � prediction
        break;
      }
      default:BL_FIX_BUG();
    }
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
  //delete (*ppPacket);// ���� ��� ������ �� �����
}
//----------------------------------------------------------------------------------
void TRoom::SendInitCoordTank(TClient* pClient)
{
  // ������ ��������� �������� �� �������� �����, ��� ������ �������� � �������������
  mPrediction.mListTank.####
  while(it!=eit)
  {

  }
  
  WriteTransportAnswer(pClient,&packet);
}
//----------------------------------------------------------------------------------
void TRoom::SendScore(TClient* pClient)
{
  TA_Score A_Score;
  A_Score.setScore0(score0);
  A_Score.setScore1(score1);
  A_Score.setTimeRest(DURATION_FIGHT_MS-mTimeAfterCountDown);
  WriteTransportAnswer(pClient,&A_Score);
}
//----------------------------------------------------------------------------------
void TRoom::SendStateObject(TClient* pClient)
{
  // ��� ������ �������� � �������������
  mPrediction.mListDestroyObject.
  while(it!=eit)
  {

  }
  // ���� �������� ����� �����, �� ������� �� ������
  WriteTransportAnswer(pClient,&packet);
}
//----------------------------------------------------------------------------------
