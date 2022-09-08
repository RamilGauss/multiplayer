#include "ClientTank.h"
#include <stddef.h>
#include "memory_operation.h"
#include <string.h>
#include "GlobalParamsTank.h"
#include "HiTimer.h"
#include "ApplicationProtocolPacketRequest.h"
#include "ApplicationProtocolPacketCmd.h"
#include "LogerDX.h"


ClientTank GlobalClientTank;

void ClientCallBackRcvPacket(void* data, int size)
{
	GlobalClientTank.RcvPacket(data, size);
}
//--------------------------------------------------------------
void ClientCallBackRcvStream(void* data, int size)
{
	GlobalClientTank.RcvStream(data, size);	
}
//--------------------------------------------------------------
void ClientCallBackRcvDisconnect(void* data, int size)
{
	GlobalClientTank.Disconnect(data, size);	
}
//--------------------------------------------------------------

ClientTank::ClientTank()
{
  mLastTimeRcv = 0;
	thread = NULL;
	mTransport.Register(ClientCallBackRcvPacket,nsCallBackType::eRcvPacket);
	mTransport.Register(ClientCallBackRcvStream,nsCallBackType::eRcvStream);
	mTransport.Register(ClientCallBackRcvDisconnect,nsCallBackType::eDisconnect);
}
//--------------------------------------------------------------
ClientTank::~ClientTank()
{
  stop();
}
//--------------------------------------------------------------
void* ClientThreadDefDisconnect(void*p)
{
	((ClientTank*)p)->ThreadDefDisconnect();
	return NULL;
}
//--------------------------------------------------------------
void ClientTank::ThreadDefDisconnect()
{
	guint32 now_time;
	SetLastTime();
	flgNeedStop = false;
	flgActive = true;
	while(!flgNeedStop)
	{
		ht_msleep(eWaitCheck);
		now_time = ht_GetMSCount();
		if(now_time>mLastTimeRcv+eWaitCheck)
		{
			flgActive = false;// чтобы не ждать свой собственный поток
			Disconnect(NULL,0);
			return;
		}
	}
	flgActive = false;
}
//--------------------------------------------------------------
void ClientTank::startThreadDefDisconnect()
{
	thread = g_thread_create(ClientThreadDefDisconnect,(gpointer)this,true,NULL);
}
//--------------------------------------------------------------
void ClientTank::start()
{
  mTransport.start();
	startThreadDefDisconnect();
}
//--------------------------------------------------------------
void ClientTank::stop()
{
	flgNeedStop = true;
	while(flgActive)
	{
		ht_msleep(eWaitThread);
	}

  mTransport.stop();
}
//--------------------------------------------------------------
bool ClientTank::IsConnect()
{
  return false;
}
//--------------------------------------------------------------
bool ClientTank::Connect(unsigned int ip_dst, unsigned int port_src, char* sNick)
{
  mIP_server = ip_dst;
  char nameLogFile[260];
  sprintf(nameLogFile,".\\clientTransport%s.log",sNick);
  mTransport.InitLog(nameLogFile);

  //--------------------------------------------
  InitLogerDX(sNick);
  //--------------------------------------------

  stop();
	mTransport.Open(port_src);

  if(mTransport.synchro(mIP_server,ServerLocalPort)==false)
		return false;

  start();
  ht_msleep(100);

  TR_Try_Connect_To_Server packet;
  packet.setNick(sNick);

  WriteTransport(&packet);

	return true;
}
//--------------------------------------------------------------
void ClientTank::PushButton(int button)
{

}
//--------------------------------------------------------------
void ClientTank::Register(CallBackRegistrator::TCallBackFunc pFunc, int type)
{
	switch(type)
	{
		case nsCallBackType::eRcvPacket:
			mCallBackPacket.Register(pFunc);
			break;
		case nsCallBackType::eRcvStream:
			mCallBackStream.Register(pFunc);
			break;
		case nsCallBackType::eDisconnect:
			mCallBackDisconnect.Register(pFunc);
			break;
		default:BL_FIX_BUG();
	}
}
//--------------------------------------------------------------
void ClientTank::Unregister(CallBackRegistrator::TCallBackFunc pFunc, int type)
{
	switch(type)
	{
		case nsCallBackType::eRcvPacket:
			mCallBackPacket.Unregister(pFunc);
			break;
		case nsCallBackType::eRcvStream:
			mCallBackStream.Unregister(pFunc);
			break;
		case nsCallBackType::eDisconnect:
			mCallBackDisconnect.Unregister(pFunc);
			break;
		default:BL_FIX_BUG();
	}
}
//--------------------------------------------------------------
void ClientTank::RcvStream(void* data, int size)
{
	SetLastTime();
	mCallBackStream.Notify(data,size);
}
//--------------------------------------------------------------
void ClientTank::RcvPacket(void* data, int size)
{
	SetLastTime();

  // обработчик на получение пакетов
  //HandlePacket(data,size);

	mCallBackPacket.Notify(data,size);
}
//--------------------------------------------------------------
void ClientTank::Disconnect(void* data, int size)
{
	stop();
	mCallBackDisconnect.Notify(data,size);
}
//--------------------------------------------------------------
void ClientTank::SendRequestListTank()
{
  TR_Get_List_Tank packet;

  WriteTransport(&packet);
}
//--------------------------------------------------------------
void ClientTank::SetCurrentTank(int i)
{
  TС_Choose_Tank packet;
  packet.setCurrentTank(i);

  WriteTransport(&packet);
}
//--------------------------------------------------------------
void ClientTank::SendRequestExitFromWait()
{
  TR_Exit_Wait packet;
  WriteTransport(&packet);
}
//--------------------------------------------------------------
void ClientTank::SendRequestInFight()
{
  TR_In_Fight packet;
  WriteTransport(&packet);
}
//--------------------------------------------------------------
void ClientTank::WriteTransport(TBasePacket* packet)
{
  TransportProtocolTank::InfoData* infoData = new TransportProtocolTank::InfoData;
  infoData->ip_dst   = mIP_server;
  infoData->port_dst = ServerLocalPort;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();

  mTransport.write(infoData);
}
//--------------------------------------------------------------
void ClientTank::SendRequestCorrectPacket()
{
  TR_Correct_Packet R_Correct_Packet;
  WriteTransport(&R_Correct_Packet);
}
//--------------------------------------------------------------
//--------------------------------------------------------------
//void HandlePacket(void* data,int size)
//{
//
//}