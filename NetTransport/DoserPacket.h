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


#ifndef DoserPacketH
#define DoserPacketH

#include <vector>

#include "INetTransport.h"
#include "BreakPacket.h"
#include "GCS.h"
#include "hArray.h"
#include "CallBackRegistrator.h"
#include "HiTimer.h"
#include "DescSendPacket.h"
#include "DoserProtocolPacket.h"

//###
#include "TypeDef.h"
//###

class TControlTraffic;

class /*###*/NET_TRANSPORT_EI TDoserPacket
{
  struct TIP_Port_ID : public TObject
  {
    TIP_Port       ip_port;
    unsigned short id;
    TIP_Port_ID()
    {
      id = 0;
    }
  };
  struct TDescBigPacket : public TIP_Port_ID
  {
  private:
    TContainer mCollect;
  public:
    std::vector<TContainer> arrPart;
    unsigned short          cntReadyPart;
    unsigned int            last_time_change;

    unsigned int sizeAll;
    TDescBigPacket()
    {
      last_time_change = ht_GetMSCount();
      sizeAll = 0;
      cntReadyPart = 0;
    }
    // true - пакет собран
    // false - не собран
    bool AddPart(unsigned short num, unsigned short c, char* part , int sizePart);
    void *GetCollectPacket(){return mCollect.GetPtr();}
    int GetCollectSize(){return mCollect.GetSize();}
    void Unlink(){mCollect.Unlink();}
    static void Free(char* p){TContainer::Free(p);}
  };

  enum
  {
    eTimeBetweenWork = 2000, // мс
    eTimeLivePacket  = 20000,// мс
  };

  unsigned int mTimeNextWork;// мс

  TControlTraffic* mControlTraffic;

  GCS mMutexCollect;
  GCS mMutexSend;

  TCallBackRegistrator mCallback;

public:

	TDoserPacket();
	~TDoserPacket();

  void Register(TCallBackRegistrator::TCallBackFunc pFunc);
  void Unregister(TCallBackRegistrator::TCallBackFunc pFunc);

  void Recv(INetTransport::TDescRecv* p);
  void Send(unsigned int ip, unsigned short port, TBreakPacket& packet, bool check );
  void Work();

  void SetControlTraffic(TControlTraffic* pCT);

private:
  // пакеты, ожидающие сборку
  TArrayObject mArrCollectBigPacket;
  
	TArrayObject mArrID_Send;

protected:

  static int SortByIP_PortID(const void* p1, const void* p2);
  static int SortByIP_Port(const void* p1, const void* p2);

  TDescBigPacket* GetDescPacketForCollect(TIP_Port_ID* p, int &index);
  TIP_Port_ID*    GetDescPacketForSend(TIP_Port_ID* p);

	void MakeList(TIP_Port& ip_port, TBreakPacket& packet, bool check, 
		            std::list<nsNetDoser::TDescSendPacket*>& listReadySend, 
								nsNetDoser::TDescSendPacket** ppArrDesc, nsNetDoser::THeaderBigPacket** ppArrHeader);
	void DestroyList(nsNetDoser::TDescSendPacket** ppArrDesc, nsNetDoser::THeaderBigPacket** ppArrHeader);

};

#endif
