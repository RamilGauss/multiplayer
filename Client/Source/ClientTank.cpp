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


TClientTank *pClient = NULL;

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

TClientTank::TClientTank()
{
  pClient = this;
  mLastTimeRcv = 0;
	thread = NULL;
	mTransport.Register(ClientCallBackRcvPacket,nsCallBackType::eRcvPacket);
	mTransport.Register(ClientCallBackRcvStream,nsCallBackType::eRcvStream);
	mTransport.Register(ClientCallBackRcvDisconnect,nsCallBackType::eDisconnect);
}
//--------------------------------------------------------------
TClientTank::~TClientTank()
{
  stop();
  pClient = NULL;
}
//--------------------------------------------------------------
void* ClientThreadDefDisconnect(void*p)
{
	((TClientTank*)p)->ThreadDefDisconnect();
	return NULL;
}
//--------------------------------------------------------------
void TClientTank::ThreadDefDisconnect()
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
void TClientTank::startThreadDefDisconnect()
{
	thread = g_thread_create(ClientThreadDefDisconnect,(gpointer)this,true,NULL);
}
//--------------------------------------------------------------
void TClientTank::start()
{
  mTransport.start();
	startThreadDefDisconnect();
}
//--------------------------------------------------------------
void TClientTank::stop()
{
	flgNeedStop = true;
	while(flgActive)
	{
		ht_msleep(eWaitThread);
	}

  mTransport.stop();
}
//--------------------------------------------------------------
bool TClientTank::IsConnect()
{
  return false;
}
//--------------------------------------------------------------
bool TClientTank::Connect(unsigned int ip_dst, unsigned int port_src, char* sNick)
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
void TClientTank::PushButton(int button)
{

}
//--------------------------------------------------------------
void TClientTank::Register(TCallBackRegistrator::TCallBackFunc pFunc, int type)
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
void TClientTank::Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type)
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
void TClientTank::RcvStream(void* data, int size)
{
	SetLastTime();
	mCallBackStream.Notify(data,size);
}
//--------------------------------------------------------------
void TClientTank::RcvPacket(void* data, int size)
{
	SetLastTime();

  // обработчик на получение пакетов
  //HandlePacket(data,size);

	mCallBackPacket.Notify(data,size);
}
//--------------------------------------------------------------
void TClientTank::Disconnect(void* data, int size)
{
	stop();
	mCallBackDisconnect.Notify(data,size);
}
//--------------------------------------------------------------
void TClientTank::SendRequestListTank()
{
  TR_Get_List_Tank packet;

  WriteTransport(&packet);
}
//--------------------------------------------------------------
void TClientTank::SetCurrentTank(int i)
{
  TС_Choose_Tank packet;
  packet.setCurrentTank(i);

  WriteTransport(&packet);
}
//--------------------------------------------------------------
void TClientTank::SendRequestExitFromWait()
{
  TR_Exit_Wait packet;
  WriteTransport(&packet);
}
//--------------------------------------------------------------
void TClientTank::SendRequestInFight()
{
  TR_In_Fight packet;
  WriteTransport(&packet);
}
//--------------------------------------------------------------
void TClientTank::WriteTransport(TBasePacket* packet)
{
  TTransportProtocol::InfoData* infoData = new TTransportProtocol::InfoData;
  infoData->ip_dst   = mIP_server;
  infoData->port_dst = ServerLocalPort;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();

  mTransport.write(infoData);
}
//--------------------------------------------------------------
void TClientTank::WriteStream(TBasePacket* packet)
{
  TTransportProtocol::InfoData* infoData = new TTransportProtocol::InfoData;
  infoData->ip_dst   = mIP_server;
  infoData->port_dst = ServerLocalPort;
  infoData->packet   = packet->getData();
  infoData->size     = packet->getSize();

  mTransport.write(infoData,false);
}
//--------------------------------------------------------------
void TClientTank::SendRequestCorrectPacket()
{
  TR_Correct_Packet R_Correct_Packet;
  WriteTransport(&R_Correct_Packet);
}
//--------------------------------------------------------------
void TClientTank::SendRequestExitFromFight()
{
  TR_Exit_Fight R_Exit_Fight;
  WriteTransport(&R_Exit_Fight);
}
//--------------------------------------------------------------
void TClientTank::SendOrientAim(float x,float y,float z)
{
  TS_Orient_Aim packet;
  packet.setXYZ(0,0,1);
  WriteStream(&packet);
}
//--------------------------------------------------------------