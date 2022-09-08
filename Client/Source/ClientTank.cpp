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


#include "ClientTank.h"
#include <stddef.h>
#include "memory_operation.h"
#include <string.h>
#include "GlobalParamsTank.h"
#include "HiTimer.h"
#include "ApplicationProtocolPacketRequest.h"
#include "ApplicationProtocolPacketCmd.h"
#include "LoggerDX.h"
#include "ApplicationProtocolPacketStream.h"


ClientTank *pClient = NULL;

void ClientCallBackRcvPacket(void* data, int size)
{
	pClient->RcvPacket(data, size);
}
//--------------------------------------------------------------
void ClientCallBackRcvStream(void* data, int size)
{
	pClient->RcvStream(data, size);	
}
//--------------------------------------------------------------
void ClientCallBackRcvDisconnect(void* data, int size)
{
	pClient->Disconnect(data, size);	
}
//--------------------------------------------------------------

ClientTank::ClientTank()
{
  pClient = this;
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
  pClient = NULL;
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
  InitLoggerDX(sNick);
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
void ClientTank::WriteStream(TBasePacket* packet)
{
  TransportProtocolTank::InfoData* infoData = new TransportProtocolTank::InfoData;
  infoData->ip_dst   = mIP_server;
  infoData->port_dst = ServerLocalPort;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();

  mTransport.write(infoData,false);
}
//--------------------------------------------------------------
void ClientTank::SendRequestCorrectPacket()
{
  TR_Correct_Packet R_Correct_Packet;
  WriteTransport(&R_Correct_Packet);
}
//--------------------------------------------------------------
void ClientTank::SendRequestExitFromFight()
{
  TR_Exit_Fight R_Exit_Fight;
  WriteTransport(&R_Exit_Fight);
}
//--------------------------------------------------------------
void ClientTank::SendOrientAim(float x,float y,float z)
{
  TS_Orient_Aim packet;
  packet.setXYZ(0,0,1);
  WriteStream(&packet);
}
//--------------------------------------------------------------