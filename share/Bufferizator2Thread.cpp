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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "Bufferizator2Thread.h"
//#include "TransportProtocolPacket.h"
#include "ApplicationProtocolDef.h"
//#include "NET_LevelClientServer.h"

#if 0

void CallBackPacketInterpretator(void* data, int size)
{
  //--------------------------------------------------------
  unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));
  switch(type)
  {
    // фильтр пакетов
    case APPL_TYPE_G_A_CORRECT_PACKET_STATE_OBJECT:
    case APPL_TYPE_G_A_CORRECT_PACKET_STATE_TANK:
    case APPL_TYPE_G_A_SCORE:
    case APPL_TYPE_G_A_EVENT_IN_FIGHT:
    {
      char* dataPacket = (char*)data + sizeof(TIP_Port);
      int sizePacket   = size - sizeof(TIP_Port);
      BL_ASSERT(pBufferizator2Thread);
      pBufferizator2Thread->RcvPacket(dataPacket,sizePacket);
      break;
    }
    default:;
  }
  //--------------------------------------------------------
}
//-------------------------------------------------------------------------------------------
void CallBackStreamInterpretator(void* data, int size)
{
  //--------------------------------------------------------
  unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));
  switch(type)
  {
    // фильтр пакетов
    case APPL_TYPE_G_S_LOAD_MAP:
      break;
    case APPL_TYPE_G_S_COUNT_DOWN:
    case APPL_TYPE_G_S_FIGHT_COORD_BULLET:
    {
      char* dataPacket = (char*)data + sizeof(TIP_Port);
      int sizePacket   = size - sizeof(TIP_Port);
      BL_ASSERT(pBufferizator2Thread);
      pBufferizator2Thread->RcvStream(dataPacket,sizePacket);
      break;
    }
    default:;
  }
  //--------------------------------------------------------
}
//-------------------------------------------------------------------------------------------
#endif
TBufferizator2Thread::TBufferizator2Thread()
:
  mQueuePacket(eCntElemPacket,eSizeElemPacket),
  mQueueStream(eCntElemStream,eSizeElemStream)
{
  //pBufferizator2Thread = this;
}
//------------------------------------------------------------------------
TBufferizator2Thread::~TBufferizator2Thread()
{
  //pBufferizator2Thread = NULL;
}
//------------------------------------------------------------------------
void TBufferizator2Thread::RcvPacket(void* dataPacket, int sizePacket)
{
  mQueuePacket.Push(dataPacket,sizePacket);
}
//------------------------------------------------------------------------
void TBufferizator2Thread::RcvStream(void* dataPacket, int sizePacket)
{
  mQueueStream.Push(dataPacket,sizePacket);
}
//------------------------------------------------------------------------
bool TBufferizator2Thread::GetPacket(void* dataPacket, int& sizePacket,guint32 &time_ms)
{
  return mQueuePacket.Pop(dataPacket,sizePacket,time_ms);
}
//------------------------------------------------------------------------
bool TBufferizator2Thread::GetStream(void* dataPacket, int& sizePacket,guint32 &time_ms)
{
  return mQueueStream.PopFresh(dataPacket,sizePacket,time_ms);
}
//------------------------------------------------------------------------
void TBufferizator2Thread::RegisterToClientTank()
{
#if 0
  GlobalClientTank.Register(CallBackPacketInterpretator,nsCallBackType::eRcvPacket);
  GlobalClientTank.Register(CallBackStreamInterpretator,nsCallBackType::eRcvStream);
#endif
  flgWasRegisterCallback = true;
}
//------------------------------------------------------------------------
void TBufferizator2Thread::UnregisterFromClientTank()
{
  if(flgWasRegisterCallback==false) return;

#if 0
  GlobalClientTank.Unregister(CallBackPacketInterpretator,nsCallBackType::eRcvPacket);
  GlobalClientTank.Unregister(CallBackStreamInterpretator,nsCallBackType::eRcvStream);
#endif
}
//------------------------------------------------------------------------
