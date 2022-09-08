#include "ServerTank.h"
#include <stddef.h>
#include "HiTimer.h"
#include "BL_Debug.h"
#include "ServerStruct.h"
#include "ApplicationProtocolPacketAnswer.h"
#include "ApplicationProtocolPacketStream.h"
#include <stdlib.h>
#include "TypeTank.h"
#include "ApplicationProtocolPacketCmd.h"
#include "Room.h"
#include "Tank.h"
#include "GlobalParamsTank.h"
#include <QMessageBox>
#include "qobject.h"
#include "Win_Func.h"


#define DEBUG_LOAD_SERVER // ������ � ���� ������� �������� �������� ������

#ifdef DEBUG_LOAD_SERVER

#define START_LOAD_TIME \
guint32 mLogLoadLastTime = ht_GetMSCount();

#define LOG_LOAD_TIME \
{ \
  guint32 now_ms = ht_GetMSCount(); \
  guint32 work_time = now_ms-mLogLoadLastTime; \
  double loadProcent = (work_time*100.0)/eTimeSleep; \
  statistica.add(loadProcent); \
  mLogLoad.WriteF("%d\n",int(statistica.aver())); \
}
#else 
  #define LOG_LOAD_TIME
  #define START_LOAD_TIME
#endif


ServerTank* pServer = NULL;

using namespace nsServerStruct;

void CallBackFuncServerRcvStream(void* data, int size)
{
  if(pServer==NULL) BL_FIX_BUG();
  char* pDataPacket = (char*)data+sizeof(TIP_Port);
  int sizePacket = size - sizeof(TIP_Port);
  TIP_Port* ip_port= (TIP_Port*)data;
  unsigned short type = *((unsigned short*)pDataPacket);
  switch(type)
  {
    case APPL_TYPE_S_ORIENT_AIM:
    {
      TS_Orient_Aim* packet = new TS_Orient_Aim;
      packet->setData(pDataPacket,sizePacket);
      pServer->AddStream(ip_port, packet);
      break;
    }
    default:;
  }
}
//----------------------------------------------------------------------------
void CallBackFuncServerRcvPacket(void* data, int size)
{
  if(pServer==NULL) BL_FIX_BUG();
	char* pDataPacket = (char*)data+sizeof(TIP_Port);
	int sizePacket = size - sizeof(TIP_Port);
	TIP_Port* ip_port= (TIP_Port*)data;
	unsigned short type = *((unsigned short*)pDataPacket);
  switch(type)
  {
    case APPL_TYPE_R_TRY_CONNECT_TO_SERVER:
		{	
			TR_Try_Connect_To_Server* packet = new TR_Try_Connect_To_Server;
			packet->setData(pDataPacket,sizePacket);
			pServer->AddPacket(ip_port, packet);
      break;
		}
    case APPL_TYPE_R_GET_LIST_TANK:
    {
      TR_Get_List_Tank* packet = new TR_Get_List_Tank;
      packet->setData(pDataPacket,sizePacket);
      pServer->AddPacket(ip_port, packet);
      break;
    }
    case APPL_TYPE_�_CHOOSE_TANK:
    {
      T�_Choose_Tank* packet = new T�_Choose_Tank;
      packet->setData(pDataPacket,sizePacket);
      pServer->AddPacket(ip_port, packet);
      break;
    }
    case APPL_TYPE_R_IN_FIGHT:
    {
      TR_In_Fight* packet = new TR_In_Fight;
      packet->setData(pDataPacket,sizePacket);
      pServer->AddPacket(ip_port, packet);
      break;
    }
    case APPL_TYPE_R_EXIT_WAIT:
    {
      TR_Exit_Wait* packet = new TR_Exit_Wait;
      packet->setData(pDataPacket,sizePacket);
      pServer->AddPacket(ip_port, packet);
      break;
    }
    case APPL_TYPE_R_CORRECT_PACKET:
    {
      TR_Correct_Packet* packet = new TR_Correct_Packet;
      packet->setData(pDataPacket,sizePacket);
      pServer->AddPacket(ip_port, packet);
      break;
    }
    case APPL_TYPE_C_KEY_EVENT:
    {
      TC_Key_Event* packet = new TC_Key_Event;
      packet->setData(pDataPacket,sizePacket);
      pServer->AddPacket(ip_port, packet);
      break;
    }
    default:;
  }
}
//----------------------------------------------------------------------------
void CallBackFuncServerDisconnect(void* ip_port, int size)
{
  if(pServer==NULL) BL_FIX_BUG();
	pServer->DisconnectClient((TIP_Port*)ip_port);
}
//----------------------------------------------------------------------------
ServerTank::ServerTank(int numNetWork):mTransport(".\\serverTransport.log"),statistica(30)
{
	mLogLoad.ReOpen(".\\serverLoad.xls");

	flgNeedRequest = false;
  mArrClients.Sort(SortFresh);
  mArrClientsFight.Sort(SortFresh);
  mArrClientsWait.Sort(SortFresh);

  mTransport.Open(ServerLocalPort,numNetWork);
  pServer = this;
  mTransport.Register(CallBackFuncServerRcvPacket, nsCallBackType::eRcvPacket);
  mTransport.Register(CallBackFuncServerRcvStream, nsCallBackType::eRcvStream);
  mTransport.Register(CallBackFuncServerDisconnect,nsCallBackType::eDisconnect);
}
//----------------------------------------------------------------------------
ServerTank::~ServerTank()
{
  pServer = NULL;
  mTransport.Unregister(CallBackFuncServerRcvPacket, nsCallBackType::eRcvPacket);
  mTransport.Unregister(CallBackFuncServerDisconnect,nsCallBackType::eDisconnect);

  mListRoom.Clear();
  mArrClientsFight.Unlink();
  mArrClientsWait.Unlink();
}
//----------------------------------------------------------------------------
void ServerTank::start()
{
	StartThread();
	mTransport.start();
}
//----------------------------------------------------------------------------
void ServerTank::stop()
{
  StopThread();
  mTransport.stop();
}
//----------------------------------------------------------------------------
void* ThreadProcServer( void* p)
{
	((ServerTank*)(p))->ThreadStreamAnswer();
	return 0;
}
//---------------------------------------------------------------------------
void ServerTank::StartThread()
{
	threadStreamAnswerCommand = g_thread_create(ThreadProcServer,
		(gpointer)this,
		true,
		NULL);
}
//----------------------------------------------------------------------------
void ServerTank::StopThread()
{
  flgNeedStop = true;
  while(flgActive)
  {
    ht_msleep(eWaitThread);
  }
}
//----------------------------------------------------------------------------
void ServerTank::ThreadStreamAnswer()
{
  flgNeedStop = false;
  flgActive = true;
  while(!flgNeedStop)
  {
    mLastTime = ht_GetMSCount();
        START_LOAD_TIME
		// �������� ��� �������������
		WorkListDisconnect();
		// ��������� �������� � ������ �� ��������
		WorkCmdRequest();
		// 
		WorkRefreshListClient();

    WorkBalanser();
    // ����� - ������
		// � ���: �������� �� �������� � ������ ����� ���������� �������
		// � ������ � � ��������: ������ ���������� � ���-�� � ��� � ����� ���-��
		WorkStream();
    // ��������� ������� �������� �� GUI
    WorkRequest();// � �������� ��� �������
		//-----------------------------------------------------------------------
        LOG_LOAD_TIME
    // ����� ������� �������
    WaitRestTime();
  }
  flgActive = false;
}
//---------------------------------------------------------------------------
void ServerTank::WaitRestTime()
{
  guint32 now_ms = ht_GetMSCount();
  if(now_ms>eTimeSleep+mLastTime) return;
  guint32 time_sleep = mLastTime + eTimeSleep - now_ms;
  ht_msleep(time_sleep);
}
//---------------------------------------------------------------------------
// �������� ����� � ������� �� ���������
void ServerTank::AddPacket(TIP_Port* ip_port, TBasePacket* packet)
{
	nsServerStruct::TPacketServer* servPacket = new nsServerStruct::TPacketServer;
	servPacket->ip      = ip_port->ip;
	servPacket->port    = ip_port->port;
	servPacket->packet  = packet;
  servPacket->ms_time = ht_GetMSCount();

	mListPacket.Add(servPacket);
}
//---------------------------------------------------------------------------
void ServerTank::AddStream(TIP_Port* ip_port, TBasePacket* packet)
{
  nsServerStruct::TPacketServer* servPacket = new nsServerStruct::TPacketServer;
  servPacket->ip      = ip_port->ip;
  servPacket->port    = ip_port->port;
  servPacket->packet  = packet;
  servPacket->ms_time = ht_GetMSCount();

  mListStream.Add(servPacket);
}
//---------------------------------------------------------------------------
void ServerTank::DisconnectClient(TIP_Port* ip_port)
{
	//�������� � ������ 
	TIP_Port* pNewIp_Port = new TIP_Port;
	pNewIp_Port->ip   = ip_port->ip;
	pNewIp_Port->port = ip_port->port;
	mListDisconnectClient.Add(pNewIp_Port);
}
//---------------------------------------------------------------------------
void ServerTank::WorkCmdRequest()
{
	nsServerStruct::TPacketServer** ppPacket = mListPacket.GetFirst();
	while(ppPacket)
	{
		TBasePacket* pPacket = (*ppPacket)->packet;
		TPacketServer** ppNext = mListPacket.Next(ppPacket);
		switch(pPacket->getType())
		{
			case APPL_TYPE_R_TRY_CONNECT_TO_SERVER:
				Appl_Type_R_Try_Connect_To_Server(ppPacket,pPacket);
				break;
      case APPL_TYPE_R_GET_LIST_TANK:
				Appl_Type_R_Get_List_Tank(ppPacket,pPacket);
        break;
      case APPL_TYPE_�_CHOOSE_TANK:
				Appl_Type_C_Choose_Tank(ppPacket,pPacket);
        break;
      case APPL_TYPE_R_IN_FIGHT:
				Appl_Type_R_In_Fight(ppPacket,pPacket);
        break;
      case APPL_TYPE_R_EXIT_WAIT:
				Appl_Type_R_Exit_Wait(ppPacket,pPacket);
        break;
    //  case APPL_TYPE_C_READY_FOR_ROOM:
				//Appl_Type_C_Ready_For_Room(ppPacket,pPacket);
    //    break;
      case APPL_TYPE_S_ORIENT_AIM:
      case APPL_TYPE_C_KEY_EVENT:
      case APPL_TYPE_R_CORRECT_PACKET:
        RoomHandler(ppPacket,pPacket);
			default:;
		}
		ppPacket = ppNext;
	}
}
//---------------------------------------------------------------------------
void ServerTank::WorkStream()
{
  TS_Garage S_Garage;
  S_Garage.setFight(mArrClientsFight.Count());
  S_Garage.setCommon(mArrClients.Count());

  TS_Wait S_Wait;
  S_Wait.setWait(mArrClientsWait.Count());
  S_Wait.setFight(mArrClientsFight.Count());
  S_Wait.setCommon(mArrClients.Count());
  // �������� �� ��� ��� � ������ � � ��������
	for(int i = 0 ; i < mArrClients.Count() ; i++ )
	{
		TClient* pClient = (TClient*)mArrClients.Get(i);
    if(pClient->flgDisconnect==false)
    {
      switch(pClient->state)
      {
        case TClient::eGarage:
        {
          WriteTransportS(pClient,&S_Garage);
          break;
        }
        case TClient::eWait:
        {
          WriteTransportS(pClient,&S_Wait);
          break;
        }
        default:;
      }
    }
	}
	// �������� �� �������� � ������ ��������� ������
	// ������� ������ ���������
  // �������� �� ��������
  TRoom **ppRoom = mListRoom.GetFirst();
  while(ppRoom)
  {
    TRoom** ppNext = mListRoom.Next(ppRoom);
    TRoom * pRoom = *ppRoom;
    if(pRoom->Work()==false)
    {
      // ������� ��������� �������� - ��� ����������
      mListRoom.Remove(ppRoom);
    }
    ppRoom = ppNext;
  }
}
//---------------------------------------------------------------------------
unsigned char ServerTank::RegisterClient(TR_Try_Connect_To_Server* tryConnect, unsigned int ip, unsigned short port)
{
  // ��, ��� �� ��� ��� ������ ���� ��� ������ � eBlackList(������ � ������ ������)

  if(statistica.aver()>double(eMaxLoadServer))
    return TA_Try_Connect_To_Server::eOverloadServer;

  if(tryConnect->getVersion()!=VERSION_PROGRAMM) 
    return TA_Try_Connect_To_Server::eOldVersion;

  char* sNick = tryConnect->getNick();

  nsServerStruct::TClient* pClient = new nsServerStruct::TClient;
  pClient->ip    = ip;
  pClient->port  = port;
  int index = mArrClients.FastSearch(&pClient,NULL,SortFresh);
  if(index!=-1)
  {
    delete pClient;
    pClient = (nsServerStruct::TClient*)mArrClients.Get(index);
    if((strcmp(pClient->sNick,sNick)==0)&&
       (pClient->ip==ip&&pClient->port==port))
    {
      // ������ ��� ���� � ������� ��������
			pClient->flgDisconnect = false;
      return pClient->state;
    }
    else
      if((strcmp(pClient->sNick,sNick)==0)||
         (pClient->ip==ip&&pClient->port==port))
      {
        return TA_Try_Connect_To_Server::eWasConnect;
      }
  }

	if(FindNickInArrClient(sNick)) return TA_Try_Connect_To_Server::eWasConnect;

  // �������� ������ �������
	int lenNick = strlen(sNick);
	pClient->sNick = (char*)malloc(lenNick+1);
	strcpy(pClient->sNick,sNick);
	pClient->ip   = ip;
	pClient->port = port;
	pClient->state = nsServerStruct::TClient::eGarage;
	pClient->flgDisconnect = false;

	mArrClients.Add(pClient);

	return TA_Try_Connect_To_Server::eConnectGarage;
}
//---------------------------------------------------------------------------
void ServerTank::WorkListDisconnect()
{
	TIP_Port** ppIP_Port = mListDisconnectClient.GetFirst();
	if(ppIP_Port==NULL) return;
	while(ppIP_Port)
	{
		TIP_Port** ppNext = mListDisconnectClient.Next(ppIP_Port);
		TIP_Port* pIP_Port = *ppIP_Port;
    nsServerStruct::TClient dClient;
    nsServerStruct::TClient* pdClient = &dClient;
    dClient.ip    = pIP_Port->ip;
    dClient.port  = pIP_Port->port;
    int index = mArrClients.FastSearch(&pdClient,NULL,SortFresh);
    if(index!=-1)
    {
      nsServerStruct::TClient* pClient = (nsServerStruct::TClient*)mArrClients.Get(index);
			pClient->flgDisconnect = true;
			pClient->time = ht_GetMSCount();
		}
    mListDisconnectClient.Remove(ppIP_Port);
		ppIP_Port = ppNext;
	}
}
//---------------------------------------------------------------------------
void ServerTank::WorkRefreshListClient()
{
	guint32 now_ms = ht_GetMSCount();
  for(int i = 0 ; i < mArrClients.Count() ; i++ )
	{
		TClient* pClient = (TClient*)mArrClients.Get(i);
		if(pClient->flgDisconnect)
		{
			if(now_ms>(pClient->time+nsServerStruct::TClient::eRemoveInterval*1000))
			{
				switch(pClient->state)
				{
					case TClient::eWait:
						mArrClientsWait.Unlink(mArrClientsWait.FastSearch(&pClient,NULL,SortFresh));
						break;
					case TClient::eFight:
						mArrClientsFight.Unlink(mArrClientsFight.FastSearch(&pClient,NULL,SortFresh));
						break;
					default:;
				}
				mArrClients.Delete(i);
			}
		}
		else
		{
			if(now_ms>(pClient->time+nsServerStruct::TClient::eTimeRefresh*1000))
			{
				pClient->time = now_ms;
				SendEcho(pClient->ip,pClient->port);
			}
		}
  }
}
//---------------------------------------------------------------------------
void ServerTank::SendEcho(unsigned int ip, unsigned short port)
{
	TA_Echo echo;
	WriteTransport(ip,port,&echo);
}
//---------------------------------------------------------------------------
int ServerTank::SortFresh(const void* p1, const void* p2)
{
  const TClient *s1 = *( const TClient **)p1;
  const TClient *s2 = *( const TClient **)p2;

  if(s1->ip>s2->ip)
    return -1;
  else 
    if(s1->ip==s2->ip)
    {
      if(s1->port>s2->port)
        return -1;
      else 
        if(s1->port==s2->port)
          return 0;
    }
    return 1;
}
//----------------------------------------------------------------------------------
void ServerTank::WorkRequest()
{
	if(flgNeedRequest==false) return;

	int cnt = mArrClients.Count();

	TDefClient* pArr = new TDefClient[cnt];
  for(int i = 0 ; i < cnt ; i++ )
  {
    TClient* pClient = (TClient*)mArrClients.Get(i);
    strcpy(pArr[i].name,pClient->sNick);
    pArr[i].ip            = pClient->ip;
    pArr[i].port          = pClient->port;
    pArr[i].state         = pClient->state;
    pArr[i].flgDisconnect = pClient->flgDisconnect;
  }
	
  mCallBackRequestListClient.Notify(pArr,cnt*sizeof(TDefClient));
  flgNeedRequest = false;
}
//----------------------------------------------------------------------------------
void ServerTank::requestListClient()
{
  flgNeedRequest = true;
}
//----------------------------------------------------------------------------------
bool ServerTank::FindNickInArrClient(char* sNick)
{
	for(int i = 0 ; i < mArrClients.Count() ; i++ )
	{
		TClient* pClient = (TClient*)mArrClients.Get(i);
		if(strcmp(pClient->sNick,sNick)==0) return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
bool ServerTank::SetListTank(TA_Get_List_Tank *answerListTank, unsigned int ip, unsigned short port)
{
  TClient oClient;
  TClient* pClient = &oClient;
  pClient->ip    = ip;
  pClient->port  = port;
  int index = mArrClients.FastSearch(&pClient,NULL,SortFresh);
  if(index==-1) return false;
  pClient = (TClient*)mArrClients.Get(index);

  unsigned char cnt = pClient->mGarage.mArrTanks.Count();
  answerListTank->setCnt(cnt);
  for(int i = 0 ; i < cnt ; i++)
  {
    TTank* pTank = (TTank*)pClient->mGarage.mArrTanks.Get(i);
    int typeTank = pTank->GetID();
    answerListTank->setTypeTank(i,typeTank);
		answerListTank->setFlgBlockTank(i,pTank->pRoom?1:0);
  }
  return true;
}
//----------------------------------------------------------------------------------
void ServerTank::WorkBalanser()
{
  // ������ ������������
  // 1. ������ ������ - ���� �������
  while(mArrClientsWait.Count()>=COUNT_COMMAND_IN_FIGHT*2)
  {
    MakeRoom();
  }

  // 2. ��� ��� �������, �� �� ��� ��������� � ���, ��� �� � ������ - ���������.
  int cnt = mArrClientsWait.Count();
  for(int i = 0 ; i < cnt ; i++)
  {
    TClient* pClient = (TClient*)mArrClientsWait.Get(i);
    if(pClient->state==TClient::eGarage)
    {
      pClient->state = TClient::eWait;
      TA_In_Fight answerFail;
      answerFail.setCode(TA_In_Fight::eWait);
      WriteTransport(pClient,&answerFail);
    }
  }
}
//----------------------------------------------------------------------------------
void ServerTank::WriteTransport(TClient* pClient,TBasePacket *packet)
{
  TransportProtocolTank::InfoData* infoData = new TransportProtocolTank::InfoData;
  infoData->ip_dst   = pClient->ip;
  infoData->port_dst = pClient->port;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();
  mTransport.write(infoData);     
}
//----------------------------------------------------------------------------------
void ServerTank::WriteTransport(nsServerStruct::TPacketServer* pServerPacket,TBasePacket *packet)
{
  TransportProtocolTank::InfoData* infoData = new TransportProtocolTank::InfoData;
  infoData->ip_dst   = pServerPacket->ip;
  infoData->port_dst = pServerPacket->port;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();
  mTransport.write(infoData);     
}
//----------------------------------------------------------------------------------
void ServerTank::WriteTransport(unsigned int ip, unsigned short port,TBasePacket *packet)
{
  TransportProtocolTank::InfoData* infoData = new TransportProtocolTank::InfoData;
  infoData->ip_dst   = ip;
  infoData->port_dst = port;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();
  mTransport.write(infoData);     
}
//----------------------------------------------------------------------------------
void ServerTank::WriteTransportS(TClient* pClient,TBasePacket *packet)
{
  TransportProtocolTank::InfoData* infoData = new TransportProtocolTank::InfoData;
  infoData->ip_dst   = pClient->ip;
  infoData->port_dst = pClient->port;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();
  mTransport.write(infoData,false);     
}
//----------------------------------------------------------------------------------
void ServerTank::WriteTransportS(nsServerStruct::TPacketServer* pServerPacket,TBasePacket *packet)
{
  TransportProtocolTank::InfoData* infoData = new TransportProtocolTank::InfoData;
  infoData->ip_dst   = pServerPacket->ip;
  infoData->port_dst = pServerPacket->port;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();
  mTransport.write(infoData,false);     
}
//----------------------------------------------------------------------------------
void ServerTank::WriteTransportS(unsigned int ip, unsigned short port,TBasePacket *packet)
{
  TransportProtocolTank::InfoData* infoData = new TransportProtocolTank::InfoData;
  infoData->ip_dst   = ip;
  infoData->port_dst = port;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();
  mTransport.write(infoData,false);     
}
//----------------------------------------------------------------------------------
int ServerTank::Appl_Type_R_Try_Connect_To_Server(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket)
{
	TR_Try_Connect_To_Server* tryConnect = (TR_Try_Connect_To_Server*)pPacket;
	// ������� � ����������� �������
	// ������ � ������ ��������
	unsigned char resRegister = RegisterClient(tryConnect,(*ppPacket)->ip,(*ppPacket)->port);
	// ��������� ����� � �����������
  if(resRegister==TClient::eFight)
  {
    nsServerStruct::TClient oClient;
    TClient* pClient = &oClient;
    pClient->ip    = (*ppPacket)->ip;
    pClient->port  = (*ppPacket)->port;
    int index = mArrClients.FastSearch(&pClient,NULL,SortFresh);
    if(index!=-1)
    {
      pClient = (nsServerStruct::TClient*)mArrClients.Get(index);
      SendPacket_A_InFight(pClient);
    }
    else BL_FIX_BUG();
  }
  else
  {
    TA_Try_Connect_To_Server answer;
    answer.setCode(resRegister);
    WriteTransport(*ppPacket,&answer);
  }
  mListPacket.Remove(ppPacket);
	return 0;
}
//----------------------------------------------------------------------------------
int ServerTank::Appl_Type_R_Get_List_Tank(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket)
{
	TR_Get_List_Tank* pRequestList = (TR_Get_List_Tank*)pPacket;
	// ������������ ������ ������
	// ��� ������ �� �� ���� �� �������, �� �.�. �� ���, �� ����� ��� ����� �� ��������� �� � ��
	TA_Get_List_Tank answerListTank;
	if(SetListTank(&answerListTank,(*ppPacket)->ip,(*ppPacket)->port))
	{
		// ���������� � ��������
		WriteTransport(*ppPacket,&answerListTank);
	}
	mListPacket.Remove(ppPacket);  
	return 0;
}
//----------------------------------------------------------------------------------
int ServerTank::Appl_Type_C_Choose_Tank(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket)
{
	TClient oClient;
	TClient* pClient = &oClient;
	pClient->ip    = (*ppPacket)->ip;
	pClient->port  = (*ppPacket)->port;
	pClient = (TClient*)mArrClients.Get(mArrClients.FastSearch(&pClient,NULL,SortFresh));
	if(pClient)
		if(pClient->state==TClient::eGarage)// ������������
		{
			T�_Choose_Tank* p�_Choose_Tank = (T�_Choose_Tank*)pPacket;
			if(pClient->mGarage.SetCurTank(p�_Choose_Tank->getCurrentTank())==false)
				BL_FIX_BUG();
		}
	mListPacket.Remove(ppPacket);
	return 0;
}
//----------------------------------------------------------------------------------
int ServerTank::Appl_Type_R_In_Fight(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket)
{
	TClient oClient;
	TClient* pClient = &oClient;
	pClient->ip    = (*ppPacket)->ip;
	pClient->port  = (*ppPacket)->port;
	pClient = (TClient*)mArrClients.Get(mArrClients.FastSearch(&pClient,NULL,SortFresh));
	if(pClient)
	{
		unsigned char codeA_In_Fight = TA_In_Fight::eFight;
		if(pClient->state==TClient::eGarage)
		{
			if(pClient->mGarage.GetPointerCurTank()->pRoom==NULL)
				mArrClientsWait.Add(pClient);// ���� �� ��������� � ���
			else
				codeA_In_Fight = TA_In_Fight::eFailBlockTank;// ���������
		}
		else
		{
			codeA_In_Fight = TA_In_Fight::eFail;
			WinMessageBox("APPL_TYPE_R_IN_FIGHT::���� �� � ������.");
		}
		if(codeA_In_Fight!=TA_In_Fight::eFight)
		{
			// ���������� � ��������
			TA_In_Fight answerFail;
			answerFail.setCode(codeA_In_Fight);
			WriteTransport(*ppPacket,&answerFail);
		}

		mListPacket.Remove(ppPacket);
	}
	return 0;
}
//----------------------------------------------------------------------------------
int ServerTank::Appl_Type_R_Exit_Wait(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket)
{
	TClient oClient;
	TClient* pClient = &oClient;
	pClient->ip    = (*ppPacket)->ip;
	pClient->port  = (*ppPacket)->port;
	int indexClientFound = mArrClientsWait.FastSearch(&pClient,NULL,SortFresh);
	pClient = (TClient*)mArrClientsWait.Get(indexClientFound);
	if(pClient)
	{
		mArrClientsWait.Unlink(indexClientFound);
		pClient->state = TClient::eGarage;
		TA_Exit_Wait answerExitWait;
		WriteTransport(pClient,&answerExitWait);
	} 

	mListPacket.Remove(ppPacket);
	return 0;
}
//----------------------------------------------------------------------------------
void ServerTank::SendPacket_A_InFight(TClient* pClient)
{
  TRoom* pRoom = pClient->GetCurRoom();
  TA_In_Fight answerFight;
  answerFight.setCode(TA_In_Fight::eFight);
  answerFight.setCodeMap(pRoom->GetIDMap());
  answerFight.setCountTank(2*COUNT_COMMAND_IN_FIGHT);

  // ���� � �������: 1 � 2 �������
  for(int i = 0 ; i < COUNT_COMMAND_IN_FIGHT*2 ; i++)
  {
    // ����� ������������� �� ������ �������
    TTank* pTank = pRoom->GetTank(i);// ����� �������� �����
    answerFight.setPointerStrNick(i,pTank->GetMasterClient()->sNick);
    answerFight.setGunType(i,pTank->mGun);
    answerFight.setTowerType(i,pTank->mTower);
    answerFight.setID_Tank(i,pTank->GetID());
  }
  //--------------------------------------
  WriteTransport(pClient,&answerFight);
}
//----------------------------------------------------------------------------------
void ServerTank::MakeRoom()
{
  // ������������ �������
  TRoom* pRoom = new TRoom;
  unsigned short id_map = GenerateID_Map();// ��������� ID
  pRoom->SetIDMap(id_map);// �������� ���� ������� � �������� ����� 
  mListRoom.Add(pRoom);
  
  for(int i = 0 ; i < COUNT_COMMAND_IN_FIGHT*2 ; i++)
  {
    // ������ �� ������ ���������
    TClient* pClient = (TClient*)mArrClientsWait.Get(0);
    mArrClientsWait.Unlink(0);

    // ������ ������ � ���
    pClient->state = TClient::eFight;
    pClient->SetCurRoom(pRoom); // ������ ������� �������
    pRoom->AddTank(pClient->mGarage.GetPointerCurTank());

    int indexInFight = mArrClientsFight.FastSearch(&pClient,NULL,SortFresh);
    if(indexInFight==-1)
      mArrClientsFight.Add(pClient);
  }

  // ��������� �������
  pRoom->MakeGroup();// ������� ������������������ ������� � ������������� ������ ������ �� ���� mGroup
  pRoom->SetTransport(&mTransport);
  pRoom->LoadMap();
  // ��������� �������� ���� �� �����
  for(int i = 0 ; i < COUNT_COMMAND_IN_FIGHT*2 ; i++)
  {
    TTank* pTank = pRoom->GetTank(i);
    if(pTank==NULL){BL_FIX_BUG();return;}
    TClient* pClient = pTank->GetMasterClient();
    SendPacket_A_InFight(pClient);
  }
}
//----------------------------------------------------------------------------------
unsigned short ServerTank::GenerateID_Map()/*���� ����*/
{
  return 0;
}
//----------------------------------------------------------------------------------
int ServerTank::RoomHandler(nsServerStruct::TPacketServer**ppPacket, TBasePacket* pPacket)
{
  // ������������� ����� ������ �� �������
  TClient oClient;
  TClient* pClient = &oClient;
  pClient->ip    = (*ppPacket)->ip;
  pClient->port  = (*ppPacket)->port;
  int indexClientFound = mArrClientsFight.FastSearch(&pClient,NULL,SortFresh);
  pClient = (TClient*)mArrClientsFight.Get(indexClientFound);
  if(pClient)
  {
    TRoom* pRoom = pClient->GetCurRoom();
    if(pRoom)
    {
      switch(pPacket->getType())
      {
        case APPL_TYPE_S_ORIENT_AIM:
        case APPL_TYPE_C_KEY_EVENT:
        case APPL_TYPE_R_CORRECT_PACKET:
          pRoom->SetPacket(*ppPacket,pClient->mGarage.GetPointerCurTank());// ��������� ������ ����� �� ��������� �������
          mListPacket.ZeroPointerElement(ppPacket);// �������� ���������, ������ �� ���������
          break;
        default:;
      }
    }
    else
      WinMessageBox("��� ������� ��� �������, pCurRoom=NULL, RoomHandler(...)");
  }
  else
    WinMessageBox("������ �� � ���, RoomHandler(...)");

  mListPacket.Remove(ppPacket);

  return 0;
}
//----------------------------------------------------------------------------------