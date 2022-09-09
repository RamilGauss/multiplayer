/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
    g_NetDoser->RecvPacket(p,s);
  g_doser_unlock();
}
//-----------------------------------------------------------------------
void FuncRecvStream(void* p, int s)
{
  g_doser_lock();
  if(g_NetDoser)
    g_NetDoser->RecvStream(p,s);
}
//-----------------------------------------------------------------------
void FuncDisconnect(void* p, int s)
{
  g_doser_lock();
  if(g_NetDoser)
    g_NetDoser->Disconnect(p,s);
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
TNetDoser::TNetDoser(char* pPathLog)
{
  g_doser_lock();
  g_NetDoser = this;
  g_doser_unlock();
  mSumSizePacket = 0;
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
	if(mTransport.Synchro(ip,port))
		return TestConnection();
	return false;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
void* ThreadDoser(void*p)
{
  ((TNetDoser*)p)->Engine();
  return NULL;
}
//----------------------------------------------------------------------------------
// суть Дозатора - выдавать трафик, так что бы не было разъединения и был собран пакет
void TNetDoser::Start()
{
  mTransport.Start();

  thread = g_thread_create(ThreadDoser,
    (gpointer)this,
    true,
    NULL);
}
//--------------------------------------------------------------------------
void TNetDoser::Stop()
{
  mTransport.Stop();
  flgNeedStop = true;
  while(flgActive)
  {
    ht_msleep(eWaitThread);
  }
}
//--------------------------------------------------------------------------
void TNetDoser::Write(unsigned int ip, unsigned short port, void* packet, int size, bool check )
{
  // Stream не надо складировать, требует немедленной отправки,
  // ну и, естественно, и размер должен быть меньше.
  if(size+GetSizeTrySend()<eSizeWakeUpThread)
    mTransport.Write(ip, port, packet, size, check);
  else
  {
    // добавить в очередь на отправку в рабочем потоке
		// раздробить и подготовить для отправки в рабочем потоке
    AddPacket(ip, port, packet, size, check);
  }
}
//--------------------------------------------------------------------------
void TNetDoser::Engine()
{
  flgNeedStop = false;
  flgActive = true;
  while(!flgNeedStop)
  {
    if(mListPacket.GetCnt())
    {// работаем
      Work();
    }
    else // спим
    {
      ht_msleep(eSleepThread);
    }
  }
  flgActive = false;
}
//----------------------------------------------------------------------------------
int TNetDoser::GetSizeTrySend()
{
  return mSumSizePacket;
}
//----------------------------------------------------------------------------------
void TNetDoser::AddPacket(unsigned int ip, unsigned short port, void* packet, int size, bool check)
{
  TPacket* pPacket = new TPacket(ip, port, packet, size, check);
  mListPacket.Add(pPacket);
  mSumSizePacket += size;
}
//----------------------------------------------------------------------------------
void TNetDoser::RemovePacket(TPacket** ppPacket)
{
  TPacket* pPacket = *ppPacket;
  int size = pPacket->size;
  mListPacket.Remove(ppPacket);
  mSumSizePacket -= size;
  BL_ASSERT(mSumSizePacket>=0);
}
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
TNetDoser::TPacket::TPacket(unsigned int _ip, unsigned short _port, 
                            void* _packet, int _size, bool _check)
{
  ip     = _ip;
  port   = _port;
  size   = _size;
  packet = new char[size];
  check  = _check;
  memcpy(packet,_packet,size);
}
//----------------------------------------------------------------------------------
void TNetDoser::TPacket::Done()
{
  delete[]packet;
  packet = NULL;
  size = 0;
}
//----------------------------------------------------------------------------------
void TNetDoser::Work()
{
  TPacket** ppPacket = mListPacket.GetFirst();
  // дробим пакет или отправляем оставшиеся части
  while(ppPacket&&!flgNeedStop)// либо пока не закончатся пакеты либо пока не остановили
  {
    TPacket* pPacket = *ppPacket;
    // "Дробление"
    // начинаем с первого пакета и по счетчику отсылаем его части
    if(Send(pPacket))// удалить только если дали отправить
      RemovePacket(ppPacket);
    // закончился пакет - смотрим есть ли еще
    // если есть повторяем процесс "Дробления"
    // если нет выходим
    ppPacket = mListPacket.GetFirst();
  }
}
//----------------------------------------------------------------------------------
bool TNetDoser::Send(TPacket* pPacket)
{
  int cnt_recv = (pPacket->size - 1)/eLimitSizePacket + 1;
  char* pPart  = (char*)pPacket->packet;
  int sizePart = pPacket->size;
  int restPacket = pPacket->size;
  for(int i = 0 ; i < cnt_recv ;)
  {
    for(int j = 0 ; j < eCntGroupSend ; j++ )
    {
      sizePart = (restPacket/eLimitSizePacket) ? eLimitSizePacket : restPacket;
      //---------------------------------------------------------------------------
      mTransport.Write(pPacket->ip, pPacket->port, pPart, sizePart, pPacket->check);
      restPacket -= sizePart;
      pPart   += eLimitSizePacket;
      i++;
      if(i==cnt_recv)
        return true;
      if(flgNeedStop) return false;
    }
    ht_msleep(eTimeSleepBetweenGroup);
  }
  return true;
}
//----------------------------------------------------------------------------------
void TNetDoser::RecvPacket(void* p, int s)
{
  // транслировать дальше
  mCallBackRecvPacket.Notify(p,s);
}
//----------------------------------------------------------------------------------
void TNetDoser::RecvStream(void* p, int s)
{
  // транслировать дальше
  mCallBackRecvStream.Notify(p,s);
}
//----------------------------------------------------------------------------------
void TNetDoser::Disconnect(void* p, int s)
{
  // транслировать дальше
  mCallBackDisconnect.Notify(p,s);
}
//----------------------------------------------------------------------------------
bool TNetDoser::TestConnection()
{
	return true;
}
//----------------------------------------------------------------------------------