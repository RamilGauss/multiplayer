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
#include <list>

#include "DoserPacket.h"
#include "DoserProtocolPacket.h"
#include "ControlTraffic.h"
#include "NetDoser.h"

using namespace nsNetDoser;
using namespace std;

TDoserPacket::TDoserPacket()
{
  mControlTraffic = NULL;
  mArrCollectBigPacket.Sort(SortByIP_PortID);
	mArrID_Send.Sort(SortByIP_Port);
  mTimeNextWork = 0;
}
//---------------------------------------------------------------------------------
TDoserPacket::~TDoserPacket()
{

}
//---------------------------------------------------------------------------------
void TDoserPacket::Register(TCallBackRegistrator::TCallBackFunc pFunc)
{
  mCallback.Register(pFunc);
}
//---------------------------------------------------------------------------------
void TDoserPacket::Unregister(TCallBackRegistrator::TCallBackFunc pFunc)
{
  mCallback.Unregister(pFunc);
}
//---------------------------------------------------------------------------------
void TDoserPacket::Recv(INetTransport::TDescRecv* p)
{
  THeaderBigPacket* pBig = (THeaderBigPacket*)p->data;
  char* pData  = p->data     + sizeof(THeaderBigPacket);
  int sizeData = p->sizeData - sizeof(THeaderBigPacket);

  TIP_Port_ID ip_port_id;
  ip_port_id.ip_port = p->ip_port;
  ip_port_id.id      = pBig->id;
  // найти запись, ассоциированную с ip, port и id пакета
  int index;
  mMutexCollect.lock();
  //----------------------------
  TDescBigPacket* pDBP = GetDescPacketForCollect(&ip_port_id, index);
  bool res = pDBP->AddPart(pBig->num, pBig->cnt, pData, sizeData);
  char* pReadyBigPacket = NULL;
  int sizeReadyBigPacket = 0;
  if(res)
  {
    pReadyBigPacket    = (char*)pDBP->GetCollectPacket();
    sizeReadyBigPacket = pDBP->GetCollectSize();
    pDBP->Unlink();
    mArrCollectBigPacket.Delete(index);
  }
  //----------------------------
  mMutexCollect.unlock();
  // если пакет собралс€, то отдать в callback функцию
  if(res)
  {
    mCallback.Notify(pReadyBigPacket, sizeReadyBigPacket);
    // освободить пам€ть из-под большого пакета
    TDescBigPacket::Free(pReadyBigPacket);
  }
}
//---------------------------------------------------------------------------------
void TDoserPacket::Send(unsigned int ip, unsigned short port, 
                        TBreakPacket& packet, bool check )
{
	TIP_Port ip_port = TIP_Port(ip, port);

	list<TDescSendPacket*> listReadySend;
  // раздробить если пакет большой
  if(packet.GetSize() > TNetDoser::eLimitSizePacket)
  {
		TDescSendPacket* pArrDesc = NULL;
		THeaderBigPacket* pArrHeader = NULL;
		MakeList( ip_port, packet, check, listReadySend, &pArrDesc, &pArrHeader);
		// отдать объекту, который контролирует трафик
		mControlTraffic->Send( ip_port, &listReadySend);
	  DestroyList( &pArrDesc, &pArrHeader);
  }
	else
	{
		THeaderSinglePacket header;
		TDescSendPacket desc;
		desc.SetCheck(check);
		desc.SetPacket(packet);
		desc.GetPacket()->PushFront( (char*)&header, sizeof(THeaderSinglePacket));
		listReadySend.push_back(&desc);
		// отдать объекту, который контролирует трафик
		mControlTraffic->Send( ip_port, &listReadySend);
	}
}
//--------------------------------------------------------------------------
void TDoserPacket::Work()
{
  unsigned int now = ht_GetMSCount();
  if(now<mTimeNextWork) return;
  // пробежка по пакетам, ожидающих сборки, поиск просроченных пакетов
  mMutexCollect.lock();

  int cnt = mArrCollectBigPacket.Count();
  for( int i = 0 ; i < cnt ; i++ )
  {
    TDescBigPacket* pDesc = (TDescBigPacket*)mArrCollectBigPacket[i];
    if(pDesc)
      if(pDesc->last_time_change < now)
      {
        mArrCollectBigPacket.Delete(i);
        i--;
        cnt--;
      }
  }
  
  mMutexCollect.unlock();
  mTimeNextWork = now + eTimeBetweenWork;
}
//---------------------------------------------------------------------------------
void TDoserPacket::SetControlTraffic(TControlTraffic* pCT)
{
  mControlTraffic = pCT;
}
//--------------------------------------------------------------------------
int TDoserPacket::SortByIP_PortID(const void* p1, const void* p2)
{
  const TIP_Port_ID *s1 = *( const TIP_Port_ID **)p1;
  const TIP_Port_ID *s2 = *( const TIP_Port_ID **)p2;

  if(s1->ip_port.ip>s2->ip_port.ip)
    return -1;
  else 
    if(s1->ip_port.ip==s2->ip_port.ip)
    {
      if(s1->ip_port.port>s2->ip_port.port)
        return -1;
      else 
        if(s1->ip_port.port==s2->ip_port.port)
        {
          if(s1->id>s2->id)
            return -1;
          else 
            if(s1->id==s2->id)
              return 0;
        }
    }
    return 1;
}
//----------------------------------------------------------------------------------
int TDoserPacket::SortByIP_Port(const void* p1, const void* p2)
{
  const TIP_Port_ID *s1 = *( const TIP_Port_ID **)p1;
  const TIP_Port_ID *s2 = *( const TIP_Port_ID **)p2;

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
TDoserPacket::TDescBigPacket* TDoserPacket::GetDescPacketForCollect(TIP_Port_ID* p, int &index)
{
  index = mArrCollectBigPacket.FastSearch(&p,NULL,SortByIP_PortID);
  TDescBigPacket* pFound = 
    (TDescBigPacket*)mArrCollectBigPacket.Get(index);
  if(pFound==NULL)
  {
    TDescBigPacket* pFresh = new TDescBigPacket;
    pFresh->ip_port = p->ip_port;
    pFresh->id      = p->id;

    mArrCollectBigPacket.Add(pFresh);
    // еще раз найти индекс
    index = mArrCollectBigPacket.FastSearch(&p,NULL,SortByIP_PortID);
    return pFresh;
  }
  return pFound;
}
//----------------------------------------------------------------------------------
TDoserPacket::TIP_Port_ID* TDoserPacket::GetDescPacketForSend(TIP_Port_ID* p)
{
	int index = mArrID_Send.FastSearch(&p,NULL,SortByIP_Port);
	TIP_Port_ID* pFound = 
		(TIP_Port_ID*)mArrID_Send.Get(index);
	if(pFound==NULL)
	{
		TIP_Port_ID* pFresh = new TIP_Port_ID;
		pFresh->ip_port = p->ip_port;

		mArrID_Send.Add(pFresh);
		return pFresh;
	}
	return pFound;
}
//----------------------------------------------------------------------------------
bool TDoserPacket::TDescBigPacket::AddPart(unsigned short num, 
                                           unsigned short c, char* part , int sizePart)
{
  last_time_change = ht_GetMSCount();// освежить врем€
  if(arrPart.size()==0)// если массив пустой
  {
    TContainer container;
    for(unsigned short i = 0 ; i < c ; i++ )
      arrPart.push_back(container);// застолбить место под массивы
  }
  arrPart[num].SetData(part,sizePart);
  cntReadyPart++;
  sizeAll += sizePart;
  // складировать в один контейнер
  if(cntReadyPart==arrPart.size())
  {
    mCollect.SetData(NULL, sizeAll);// резерв пам€ти
    char* pBegin = (char*)mCollect.GetPtr();
    char* pDst   = pBegin;
    for(unsigned short i = 0 ; i < c ; i++ )
    {
      char* pSrc = (char*)arrPart[i].GetPtr();
      int iSize = arrPart[i].GetSize();
      memcpy(pDst, pSrc, iSize);
      pDst = pBegin + iSize;
    }
    return true;
  }
  return false;
}
//----------------------------------------------------------------------------------
// ќчень сложна€ функци€!

// пакет представл€ет собой список массивов пам€ти.
// в сумме эта пам€ть превышает максимально допустимый размер пакета.
// задача данной функции таким образом создать новые BreakPacket,
// чтобы внутри каждого из них сумма пам€ти была меньше максимально допустимого размера.
// это достигаетс€ путем добавлени€ указателей в новый BreakPacket на области пам€ти,
// наход€щихс€ в области старого BreakPacket.
void TDoserPacket::MakeList(TIP_Port& ip_port, TBreakPacket& packet, bool check, 
														list<TDescSendPacket*>& listReadySend, 
														TDescSendPacket** ppArrDesc, THeaderBigPacket** ppArrHeader)
{
	mMutexSend.lock();
	//-----------------------
	TIP_Port_ID ip_port_id;
	ip_port_id.ip_port = ip_port;
	TIP_Port_ID* pFindIP_Port = GetDescPacketForSend(&ip_port_id);
	unsigned short id = pFindIP_Port->id++;
	//-----------------------
	mMutexSend.unlock();

  int sizePacket = packet.GetSize();
  int cPart = (sizePacket-1)/TNetDoser::eLimitSizePacket + 1;
	ppArrDesc[0]   = new TDescSendPacket[cPart];
	ppArrHeader[0] = new THeaderBigPacket[cPart];
  //------------------------------
  list<TContainerPtr>::iterator bit = packet.GetList()->begin();
  list<TContainerPtr>::iterator eit = packet.GetList()->end();
  char* pBelow = NULL;
  int rest     = 0;// остаток от предыдущей части
  for( int i = 0 ; i < cPart ; i++)
	{
    BL_ASSERT(bit!=eit);
    if(rest==0)
    {
      if(bit->GetSize() > TNetDoser::eLimitSizePacket)// избыток
      {
        // по€вл€етс€ избыток, rest увеличиваетс€
        ppArrDesc[0][i].GetPacket()->PushBack( (char*)bit->GetPtr(), TNetDoser::eLimitSizePacket);
        rest = bit->GetSize() - TNetDoser::eLimitSizePacket;
        pBelow = (char*)bit->GetPtr() + TNetDoser::eLimitSizePacket;// сдвиг от начала блока
      }
      else if(bit->GetSize()==TNetDoser::eLimitSizePacket)// равно
      {
        ppArrDesc[0][i].GetPacket()->PushBack( (char*)bit->GetPtr(), bit->GetSize());
        bit++;
      }
      else// меньше
      {
        // пытаемс€ добрать до нужной суммы
        int commonSize = 0;
        while(1)
        {
          if(commonSize + bit->GetSize()>TNetDoser::eLimitSizePacket)// избыток
          {
            int sizeLastBlock = TNetDoser::eLimitSizePacket - commonSize;
            ppArrDesc[0][i].GetPacket()->PushBack( (char*)bit->GetPtr(), sizeLastBlock);
            rest = bit->GetSize() - sizeLastBlock;
            pBelow = (char*)bit->GetPtr() + sizeLastBlock;// сдвиг от начала блока 
            break;
          }
          else if(commonSize + bit->GetSize()==TNetDoser::eLimitSizePacket)// равно
          {
            ppArrDesc[0][i].GetPacket()->PushBack( (char*)bit->GetPtr(), bit->GetSize());
            bit++;
            break;
          }
          else// недостаточно
          {
            ppArrDesc[0][i].GetPacket()->PushBack( (char*)bit->GetPtr(), bit->GetSize());
            commonSize += bit->GetSize();
            bit++;
            // вполне возможно что и не хватит
            if(bit==eit) break;
          }
        }
      }
    }//------------------------------
    else
    {
      if(rest > TNetDoser::eLimitSizePacket)// избыток
      {
        // по€вл€етс€ избыток, rest увеличиваетс€
        ppArrDesc[0][i].GetPacket()->PushBack( pBelow, TNetDoser::eLimitSizePacket);
        rest   -= TNetDoser::eLimitSizePacket;
        pBelow += TNetDoser::eLimitSizePacket;// сдвиг от начала блока
      }
      else if(rest == TNetDoser::eLimitSizePacket)// равно
      {
        // по€вл€етс€ избыток, rest увеличиваетс€
        ppArrDesc[0][i].GetPacket()->PushBack( pBelow, rest);
        rest = 0;
        bit++;
      }
      else// меньше
      {
        ppArrDesc[0][i].GetPacket()->PushBack( pBelow, rest);
        // пытаемс€ добрать до нужной суммы
        int commonSize = rest;
        rest = 0;
        bit++;// следующий блок
        if(bit!=eit)
          while(1)
          {
            if(commonSize + bit->GetSize() > TNetDoser::eLimitSizePacket)// избыток
            {
              int sizeLastBlock = TNetDoser::eLimitSizePacket - commonSize;
              ppArrDesc[0][i].GetPacket()->PushBack( (char*)bit->GetPtr(), sizeLastBlock);
              rest = bit->GetSize() - sizeLastBlock;
              pBelow = (char*)bit->GetPtr() + sizeLastBlock;// сдвиг от начала блока 
              break;
            }
            else if(commonSize + bit->GetSize()==TNetDoser::eLimitSizePacket)// равно
            {
              ppArrDesc[0][i].GetPacket()->PushBack( (char*)bit->GetPtr(), bit->GetSize());
              bit++;
              break;
            }
            else// недостаточно
            {
              ppArrDesc[0][i].GetPacket()->PushBack( (char*)bit->GetPtr(), bit->GetSize());
              commonSize += bit->GetSize();
              bit++;
              // вполне возможно что и не хватит
              if(bit==eit) break;
            }
          }
      }
    }
    //-----------------------------------------------------
    // заголовок
    ppArrDesc[0][i].GetPacket()->PushFront((char*)&(ppArrHeader[0][i]), sizeof(THeaderBigPacket));
		ppArrHeader[0][i].cnt = cPart;
		ppArrHeader[0][i].num = i;
		ppArrHeader[0][i].id  = id;
		ppArrDesc[0][i].SetCheck(check);
    listReadySend.push_back(&ppArrDesc[0][i]);
	}
}
//----------------------------------------------------------------------------------
void TDoserPacket::DestroyList(TDescSendPacket** ppArrDesc, THeaderBigPacket** ppArrHeader)
{
  // освободить ресурсы, которые были выделены в MakeList
	delete[](ppArrDesc[0]);
	ppArrDesc[0] = NULL;
	delete[](ppArrHeader[0]);
	ppArrHeader[0] = NULL;
}
//----------------------------------------------------------------------------------
