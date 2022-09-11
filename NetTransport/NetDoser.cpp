/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#include "NetDoser.h"
#include "HiTimer.h"
#include "GCS.h"
#include "DoserProtocolPacket.h"
#include "SinglePacketNetDoser.h"
#include "BigPacketNetDoser.h"

using namespace nsNetDoser;
//-----------------------------------------------------------------------
GCS g_doser_gcs;
void g_doser_lock()  {g_doser_gcs.lock();}
void g_doser_unlock(){g_doser_gcs.unlock();}
//-----------------------------------------------------------------------
static TNetDoser* g_NetDoser = NULL;
//-----------------------------------------------------------------------
void FuncRecv(void* p, int s)
{
  g_doser_lock();
  if(g_NetDoser)
    g_NetDoser->Analiz((INetTransport::TDescRecv*)p,s);
  g_doser_unlock();
}
//-----------------------------------------------------------------------
void FuncLostPacket(void* p, int s)
{
	g_doser_lock();
	if(g_NetDoser)
		g_NetDoser->LostPacket((INetTransport::TLostPacket*)p,s);
	g_doser_unlock();
}
//-----------------------------------------------------------------------
void FuncDisconnect(void* p, int s)
{
  g_doser_lock();
  if(g_NetDoser)
    g_NetDoser->Disconnect((TIP_Port*)p,s);
  g_doser_unlock();
}
//-----------------------------------------------------------------------
void FuncRecvFromDoserPacket(void *p, int s)
{
  g_NetDoser->RecvBigPacket((INetTransport::TDescRecv*)p, s);
}
//-----------------------------------------------------------------------
TNetDoser::TNetDoser(char* pPathLog) : INetTransport(pPathLog)
{
  flgActive = false;
  g_doser_lock();
  g_NetDoser = this;
  g_doser_unlock();

  mDoserPacket.Register(FuncRecvFromDoserPacket);
  mDoserPacket.SetControlTraffic(&mControlTrafficTo);
	
	mControlTrafficTo.SetTransport(&mTransport);
	//mControlTrafficFrom.SetTransport(&mTransport);
}
//--------------------------------------------------------------------------
TNetDoser::~TNetDoser()
{
  mDoserPacket.Unregister(FuncRecvFromDoserPacket);

  g_doser_lock();
  g_NetDoser = NULL;
  g_doser_unlock();
  Stop();
}
//--------------------------------------------------------------------------
void TNetDoser::InitLog(char* pPathLog)
{
  mTransport.InitLog(pPathLog);
}
//--------------------------------------------------------------------------
bool TNetDoser::Open(unsigned short port, unsigned char numNetWork)
{
	return mTransport.Open(port, numNetWork);
}
//--------------------------------------------------------------------------
void TNetDoser::Register(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type)
{
  TCallBackRegistrator::TCallBackFunc pDoserFunc = NULL;
  switch(type)
  {
    case eRecv:
      pDoserFunc = FuncRecv;
      mCallBackRecv.Register(pFunc);
      break;
		case eLostPacket:
      pDoserFunc = FuncLostPacket;
			mCallBackLostPacket.Register(pFunc);
			break;
    case eDisconnect:
      pDoserFunc = FuncDisconnect;
      mCallBackDisconnect.Register(pFunc);
      break;
  }
  mTransport.Register(pDoserFunc,type);
}
//--------------------------------------------------------------------------
void TNetDoser::Unregister(TCallBackRegistrator::TCallBackFunc pFunc, eTypeCallback type)
{
  TCallBackRegistrator::TCallBackFunc pDoserFunc = NULL;
  switch(type)
  {
    case eRecv:
      pDoserFunc = FuncRecv;
      mCallBackRecv.Unregister(pFunc);
      break;
		case eLostPacket:
      pDoserFunc = FuncLostPacket;
			mCallBackLostPacket.Unregister(pFunc);
			break; 
		case eDisconnect:
      pDoserFunc = FuncDisconnect;
      mCallBackDisconnect.Unregister(pFunc);
      break;
  }
  mTransport.Unregister(pDoserFunc,type);
}
//--------------------------------------------------------------------------
bool TNetDoser::Synchro(unsigned int ip, unsigned short port)
{
	return mTransport.Synchro(ip,port);
}
//--------------------------------------------------------------------------
void* ThreadDoser(void*p)
{
  ((TNetDoser*)p)->Engine();
  return NULL;
}
//----------------------------------------------------------------------------------
// суть ƒозатора - выдавать максимально возможный трафик, так что бы не было разъединени€ и
// передать большой пакет (разбитие большого пакета на меньшие)
void TNetDoser::Start()
{
	mTransport.Start();
  thread = g_thread_create(ThreadDoser,
    (gpointer)this,
    true,
    NULL);
  while(IsActive()==false)
    ht_msleep(eWaitActivation);
}
//--------------------------------------------------------------------------
void TNetDoser::Stop()
{
  mTransport.Stop();
  flgNeedStop = true;
  while(IsActive())
    ht_msleep(eWaitThread);
}
//--------------------------------------------------------------------------
bool TNetDoser::IsActive()
{
	return flgActive;
}
//--------------------------------------------------------------------------
void TNetDoser::Send(unsigned int ip, unsigned short port, 
                     TBreakPacket& packet,
                     bool check )
{
  mDoserPacket.Send(ip, port, packet, check);
}
//--------------------------------------------------------------------------
void TNetDoser::Engine()
{
  flgNeedStop = false;
  flgActive = true;
  while(!flgNeedStop)
  {
    if(mControlTrafficTo.Work()==false)// если не работать, то спать
      ht_msleep(eSleepThread);
    mDoserPacket.Work();
  }
  flgActive = false;
}
//----------------------------------------------------------------------------------
void TNetDoser::Disconnect(TIP_Port* p, int s)
{
  mCallBackDisconnect.Notify(p,s);// транслировать дальше
}
//----------------------------------------------------------------------------------
void TNetDoser::Analiz(TDescRecv* p, int s)
{
	//ControlTrafficFrom(p);
	// анализ содержимого пакетов и прин€тие решени€ по настройке трафика
  // и сборка больших пакетов
  THeaderBasePacket* pPacket = (THeaderBasePacket*)(p->data);
  switch(pPacket->type)
  {
    case eBigPacket:
      // собрать пакет
      mDoserPacket.Recv(p);
      break;
    case eSinglePacket:
      NotifyRecvSinglePacket(p);
      break;
    //case eOverload:
    //  // отреагировать на перегрузку
    //  mControlTrafficTo.Recv(p);
    //  break;
    default:BL_FIX_BUG();
  }
}
//----------------------------------------------------------------------------------
void TNetDoser::NotifyRecvSinglePacket(TDescRecv* p)
{
  TDescRecv descRecv = *p;
  descRecv.data     += sizeof(THeaderSinglePacket);
  descRecv.sizeData -= sizeof(THeaderSinglePacket);
  mCallBackRecv.Notify(&descRecv,sizeof(TDescRecv)); // транслировать дальше
}
//----------------------------------------------------------------------------------
void TNetDoser::RecvBigPacket(TDescRecv* p, int s)
{
  // пришло от DoserPacket, собранный пакет
  // не содержит заголовков, "чистые" данные
  mCallBackRecv.Notify(p,sizeof(TDescRecv)); // транслировать дальше
}
//----------------------------------------------------------------------------------
//void TNetDoser::ControlTrafficFrom(TDescRecv* p)
//{
//	THeaderBasePacket* pPacket = (THeaderBasePacket*)(p->data);
//	switch(pPacket->type)
//	{
//		case eBigPacket:
//		case eSinglePacket:
//			mControlTrafficFrom.Recv(p);
//			break;
//		default:;
//	}
//}
//----------------------------------------------------------------------------------
void TNetDoser::LostPacket(INetTransport::TLostPacket* p, int s)
{
	mCallBackLostPacket.Notify(p,s);
}
//----------------------------------------------------------------------------------
