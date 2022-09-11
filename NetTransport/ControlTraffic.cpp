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


#include "ControlTraffic.h"
#include "DoserProtocolPacket.h"
#include "AutomatRegularSpeed.h"

using namespace nsNetDoser;
using namespace std;

TControlTraffic::TControlTraffic() : mQEvent(eSizeQueue)
{
  mTransport = NULL;
  mArrConnect.Sort(SortConnect);

}
//---------------------------------------------------------------------------------
TControlTraffic::~TControlTraffic()
{

}
//---------------------------------------------------------------------------------
//void TControlTraffic::LostPacket(INetTransport::TLostPacket* p)
//{
//	// запомнить в очереди, в Work обработается это событие
//	TEvent event;
//	event.ip_port = p->mIP_Port;
//	event.cntTry  = p->mCntTry;
//	mQEvent.Push(&event, sizeof(event));
//}
//---------------------------------------------------------------------------------
void TControlTraffic::Send(TIP_Port& ip_port, list<TDescSendPacket*>* pLDSP)
{
  mConnectMutex.lock();
  // найти описание данного соединения
  // узнать у автомата можно ли сейчас отправить пакет
	TDescConnect* pDesc = GetDescConnect(ip_port.ip, ip_port.port);
	list<TDescSendPacket*>::iterator bit = pLDSP->begin();
	list<TDescSendPacket*>::iterator eit = pLDSP->end();
	while(bit!=eit)
	{
		if(pDesc->GetAutomat()->AddInQueue((*bit)->GetPacket()->GetSize()))//возможность мгновенной отправки
		{
			pDesc->GetAutomat()->Add(*bit);// копировать содержимое пакета внутрь
			mActiveConnect.insert(pDesc); 
		}
		else
    {
		  TBreakPacket bp = *((*bit)->GetPacket());// копия, сверху освободят память оригинала
      bp.Collect();                            // сборка
      void* p  = bp.GetCollectPtr();
      int size = bp.GetSize();
      ((THeaderSinglePacket*)p)->SetTime();// выставить время отправления
      bp.UnlinkPart();                     // отцепиться от памяти оригинала
      bp.PushBack((char*)p, size);         // задать часть заново
			mTransport->Send(ip_port.ip, ip_port.port, bp, (*bit)->GetCheck());
    }
		bit++;
	}
  mConnectMutex.unlock();
}
//---------------------------------------------------------------------------------
// Send и Work должны быть взаимно блокирующими
bool TControlTraffic::Work()
{
  mConnectMutex.lock();
	
	// обработать события из очереди
	HandleEvent();

	if(mActiveConnect.size()==0) {mConnectMutex.unlock();return false;}
	unsigned int beginWork = ht_GetMSCount();

  // параметры и кол-во активных
  TSetPtrIt bit = mActiveConnect.begin();
  TSetPtrIt eit = mActiveConnect.end();
  // отправка пакетов
	while(bit!=eit)
	{
		if((*bit)->GetAutomat()->NeedSend())
		{
			(*bit)->GetAutomat()->Send();
			bit++;
		}
		else
		{
			TSetPtrIt nit = bit;// удалить из активных соединений
			nit++;
			mActiveConnect.erase(bit);
			bit = nit;
		}
		unsigned int now = ht_GetMSCount();
		if( beginWork + eTimeWork < now )// Gauss дать возможность потоку отправки втиснуть в очередь свой пакет
			break;
	}
  mConnectMutex.unlock();
  return true;
}
//---------------------------------------------------------------------------------
void TControlTraffic::SetTransport(INetTransport* pNT)
{
  mTransport = pNT;
}
//---------------------------------------------------------------------------------
int TControlTraffic::SortConnect(const void* p1, const void* p2)
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
int TControlTraffic::SortConnectByIP_Port(const void* p1, const void* p2)
{
  const TDescConnect *s1 = *( const TDescConnect **)p1;
  const TIP_Port *s2     = *( const TIP_Port **)p2;

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
TDescConnect* TControlTraffic::GetDescConnect(unsigned int ip, unsigned short port)
{
  TIP_Port ip_port(ip,port);
  TIP_Port* pIP_Port = &ip_port;

  TDescConnect* pFoundFresh = 
    (TDescConnect*)mArrConnect.Get(mArrConnect.FastSearch(&pIP_Port,NULL,SortConnectByIP_Port));
  if(pFoundFresh==NULL)
  {
    TDescConnect* pFresh = new TDescConnect(&ip_port);
    pFresh->GetAutomat()->SetTransport((INetTransport*)mTransport);

    mArrConnect.Add(pFresh);
    return pFresh;
  }
  return pFoundFresh;
}
//----------------------------------------------------------------------------------
void TControlTraffic::HandleEvent()
{
	TEvent event;
	int size;
	while(mQEvent.Pop(&event, size))
	{
		// найти соединение и изменить параметры скорости
		TDescConnect* pDesc = GetDescConnect(event.ip_port.ip, event.ip_port.port);
		pDesc->GetAutomat()->Overload(event.cntTry);
	}
}
//----------------------------------------------------------------------------------