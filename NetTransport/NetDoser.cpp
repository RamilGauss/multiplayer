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
void FuncRecvPacket(void* p, int s)
{
  g_doser_lock();
  if(g_NetDoser)
    g_NetDoser->RecvPacket((INetTransport::TDescRecv*)p,s);
  g_doser_unlock();
}
//-----------------------------------------------------------------------
void FuncRecvStream(void* p, int s)
{
  g_doser_lock();
  if(g_NetDoser)
    g_NetDoser->RecvStream((INetTransport::TDescRecv*)p,s);
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
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TNetDoser::TNetDoser(char* pPathLog) : INetTransport(pPathLog)
{
  flgActive = false;
  g_doser_lock();
  g_NetDoser = this;
  g_doser_unlock();

  mArrConnect.Sort(SortConnect);
}
//--------------------------------------------------------------------------
TNetDoser::~TNetDoser()
{
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
void TNetDoser::Register(TCallBackRegistrator::TCallBackFunc pFunc, int type)
{
  TCallBackRegistrator::TCallBackFunc pDoserFunc = NULL;
  switch(type)
  {
    case eRcvPacket:
      pDoserFunc = FuncRecvPacket;
      mCallBackRecvPacket.Register(pFunc);
      break;
    case eRcvStream:
      pDoserFunc = FuncRecvStream;
      mCallBackRecvStream.Register(pFunc);
      break;
    case eDisconnect:
      pDoserFunc = FuncDisconnect;
      mCallBackDisconnect.Register(pFunc);
      break;
    default:BL_FIX_BUG();
  }
  mTransport.Register(pDoserFunc,type);
}
//--------------------------------------------------------------------------
void TNetDoser::Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type)
{
  TCallBackRegistrator::TCallBackFunc pDoserFunc = NULL;
  switch(type)
  {
    case eRcvPacket:
      pDoserFunc = FuncRecvPacket;
      mCallBackRecvPacket.Unregister(pFunc);
      break;
    case eRcvStream:
      pDoserFunc = FuncRecvStream;
      mCallBackRecvStream.Unregister(pFunc);
      break;
    case eDisconnect:
      pDoserFunc = FuncDisconnect;
      mCallBackDisconnect.Unregister(pFunc);
      break;
    default:BL_FIX_BUG();
  }
  mTransport.Unregister(pDoserFunc,type);
}
//--------------------------------------------------------------------------
bool TNetDoser::Synchro(unsigned int ip, unsigned short port)
{
	return mTransport.Synchro(ip,port);
}
//--------------------------------------------------------------------------
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
  {
    ht_msleep(eWaitThread);
  }
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
  int sizePacket = packet.GetSize();
  // найти описание соединени€
  TDescConnect* pDescConnect = GetDescConnect(ip, port);
  // узнать надо ли помещать в автомат пакеты дл€ отправки из рабочего потока
  bool resAdd = pDescConnect->GetAutomat()->AddInQueue(sizePacket);
  if(resAdd)
  {
    // создать описание пакетов и поместить в список на отправку. 
    // Ёти пакеты отправ€тс€ в рабочем потоке.

  }
  else
  {
    if(sizePacket>eLimitSizePacket)
      SendBigPacket(ip, port, packet, check);
    else
      SendSinglePacket(ip, port, packet, check);
  }
}
//--------------------------------------------------------------------------
void TNetDoser::Engine()
{
  flgNeedStop = false;
  flgActive = true;
  while(!flgNeedStop)
  {
    if(Work()==false)// если надо работать, то пропуск сна
      ht_msleep(eSleepThread);
  }
  flgActive = false;
}
//----------------------------------------------------------------------------------
//int TNetDoser::GetSizeTrySend()
//{
//  return mSumSizePacket;
//}
////----------------------------------------------------------------------------------
//void TNetDoser::AddPacket(unsigned int ip, unsigned short port, void* packet, int size, bool check)
//{
//  TPacket* pPacket = new TPacket(ip, port, packet, size, check);
//  mListPacket.Add(pPacket);
//  mSumSizePacket += size;
//}
////----------------------------------------------------------------------------------
//void TNetDoser::RemovePacket(TPacket** ppPacket)
//{
//  TPacket* pPacket = *ppPacket;
//  int size = pPacket->size;
//  mListPacket.Remove(ppPacket);
//  mSumSizePacket -= size;
//  BL_ASSERT(mSumSizePacket>=0);
//}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
bool TNetDoser::Work()
{
//  TPacket** ppPacket = mListPacket.GetFirst();
//  // дробим пакет или отправл€ем оставшиес€ части
//  while(ppPacket&&!flgNeedStop)// либо пока не закончатс€ пакеты либо пока не остановили
//  {
//    TPacket* pPacket = *ppPacket;
//    // "ƒробление"
//    // начинаем с первого пакета и по счетчику отсылаем его части
//    if(Send(pPacket))// удалить только если дали отправить
//      RemovePacket(ppPacket);
//    // закончилс€ пакет - смотрим есть ли еще
//    // если есть повтор€ем процесс "ƒроблени€"
//    // если нет выходим
//    ppPacket = mListPacket.GetFirst();
//  }
  return false;
}
//----------------------------------------------------------------------------------
//bool TNetDoser::Send(TPacket* pPacket)
//{
  //int cnt_recv = (pPacket->size - 1)/eLimitSizePacket + 1;
  //char* pPart  = (char*)pPacket->packet;
  //int sizePart = pPacket->size;
  //int restPacket = pPacket->size;
  //for(int i = 0 ; i < cnt_recv ;)
  //{
  //  for(int j = 0 ; j < eCntGroupSend ; j++ )
  //  {
  //    sizePart = (restPacket/eLimitSizePacket) ? eLimitSizePacket : restPacket;
  //    //---------------------------------------------------------------------------
  //    mTransport.Send(pPacket->ip, pPacket->port, pPart, sizePart, pPacket->check);
  //    restPacket -= sizePart;
  //    pPart   += eLimitSizePacket;
  //    i++;
  //    if(i==cnt_recv)
  //      return true;
  //    if(flgNeedStop) return false;
  //  }
  //  ht_msleep(eTimeSleepBetweenGroup);
  //}
  //return true;
//}
//----------------------------------------------------------------------------------
void TNetDoser::RecvPacket(TDescRecv* p, int s)
{
	// анализ содержимого пакетов и прин€тие решени€ по настройке трафика
  // и сборка больших пакетов
  Analiz(p, s);
}
//----------------------------------------------------------------------------------
void TNetDoser::RecvStream(TDescRecv* p, int s)
{
  NotifySinglePacket(p, s, mCallBackRecvStream);
}
//----------------------------------------------------------------------------------
void TNetDoser::Disconnect(TIP_Port* p, int s)
{
  // транслировать дальше
  mCallBackDisconnect.Notify(p,s);
}
//----------------------------------------------------------------------------------
void TNetDoser::Analiz(TDescRecv* p, int s)
{
  THeaderBasePacket* pPacket = (THeaderBasePacket*)(p->data);
  switch(pPacket->type)
  {
    case eBigPacket:
      // дособрать пакет
      break;
    case eSinglePacket:
      NotifySinglePacket(p, s, mCallBackRecvPacket);
      break;
    case eOverload:
      // отреагировать на перегрузку
      break;
    default:BL_FIX_BUG();
  }
}
//----------------------------------------------------------------------------------
void TNetDoser::NotifySinglePacket(TDescRecv* p, int s, TCallBackRegistrator& callback)
{
  TDescRecv descRecv;
  descRecv.ip_port  = p->ip_port;
  descRecv.data     = p->data     + sizeof(THeaderSinglePacket);
  descRecv.sizeData = p->sizeData - sizeof(THeaderSinglePacket);
  callback.Notify(&descRecv,sizeof(descRecv)); // транслировать дальше
}
//----------------------------------------------------------------------------------
int TNetDoser::SortConnect(const void* p1, const void* p2)
{
  const TDescConnect *s1 = *( const TDescConnect **)p1;
  const TDescConnect *s2 = *( const TDescConnect **)p2;

  if(s1->GetIP()>s2->GetIP())
    return -1;
  else 
    if(s1->GetIP()==s2->GetIP())
    {
      if(s1->GetPort()>s2->GetPort())
        return -1;
      else 
        if(s1->GetPort()==s2->GetPort())
          return 0;
    }
    return 1;
}
//----------------------------------------------------------------------------------
int TNetDoser::SortConnectByIP_Port(const void* p1, const void* p2)
{
  const TDescConnect *s1 = *( const TDescConnect **)p1;
  const TIP_Port *s2 = *( const TIP_Port **)p2;

  if(s1->GetIP()>s2->ip)
    return -1;
  else 
    if(s1->GetIP()==s2->ip)
    {
      if(s1->GetPort()>s2->port)
        return -1;
      else 
        if(s1->GetPort()==s2->port)
          return 0;
    }
    return 1;
}
//----------------------------------------------------------------------------------
int TNetDoser::SortBigPacket(const void* p1, const void* p2)
{
	const TCollectBigPacket *s1 = *( const TCollectBigPacket **)p1;
	const TCollectBigPacket *s2 = *( const TCollectBigPacket **)p2;

	if(s1->ip_port.ip>s2->ip_port.ip)
		return -1;
	else 
		if(s1->ip_port.ip==s2->ip_port.ip)
		{
			if(s1->ip_port.port>s2->ip_port.port)
				return -1;
			else 
				if(s1->ip_port.port==s2->ip_port.port)
					return 0;
		}
		return 1;
}
//----------------------------------------------------------------------------------
int TNetDoser::SortBigPacketByIP_Port(const void* p1, const void* p2)
{
	const TCollectBigPacket *s1 = *( const TCollectBigPacket **)p1;
	const TIP_Port *s2 = *( const TIP_Port **)p2;

	if(s1->ip_port.ip>s2->ip)
		return -1;
	else 
		if(s1->ip_port.ip==s2->ip)
		{
			if(s1->ip_port.port>s2->port)
				return -1;
			else 
				if(s1->ip_port.port==s2->port)
					return 0;
		}
		return 1;
}
//----------------------------------------------------------------------------------
TDescConnect* TNetDoser::GetDescConnect(unsigned int ip, unsigned short port)
{
  TIP_Port ip_port(ip,port);
  TIP_Port* pIP_Port = &ip_port;

  lockSendRcv();
  TDescConnect* pFoundFresh = 
    (TDescConnect*)mArrConnect.Get(mArrConnect.FastSearch(&pIP_Port,NULL,SortConnectByIP_Port));
  unlockSendRcv();
  if(pFoundFresh==NULL)
  {
    TDescConnect* pFresh = new TDescConnect;
    pFresh->GetAutomat()->SetTransport(&mTransport);
    *(pFresh->GetIP_Port()) = ip_port;

    lockSendRcv();
    mArrConnect.Add(pFresh);
    unlockSendRcv();
    return pFresh;
  }
  return pFoundFresh;
}
//----------------------------------------------------------------------------------
void TNetDoser::SendBigPacket(unsigned int ip, unsigned short port, 
                              TBreakPacket& packet, bool check)
{
  //THeaderBigPacket header;
  //header.type    = eBigPacket;
  //header.time_ms = ht_GetMSCount();
  //TBigPacketNetDoser bigPacket;
  //bigPacket.SetHeader(header);
  //bigPacket.SetData(packet,size);
  //int sizeData;
  //void* pData = bigPacket.Get(sizeData);
}
//----------------------------------------------------------------------------------
void TNetDoser::SendSinglePacket(unsigned int ip, unsigned short port, 
                                 TBreakPacket& packet, bool check)
{
  THeaderSinglePacket header;
  header.SetTime();
  packet.PushFront((char*)&header, sizeof(header));
  mTransport.Send(ip, port, packet, check);
}
//----------------------------------------------------------------------------------
void TNetDoser::Lock(void* pLocker)
{
  GCS* pGCS = (GCS*)pLocker;
  pGCS->lock();
}
//----------------------------------------------------------------------------------
void TNetDoser::Unlock(void* pLocker)
{
  GCS* pGCS = (GCS*)pLocker;
  pGCS->unlock();
}
//----------------------------------------------------------------------------------

