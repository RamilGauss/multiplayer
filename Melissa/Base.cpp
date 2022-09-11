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

#include "Base.h"
#include "Logger.h"
#include "ManagerSession.h"
#include "DescRecvSession.h"
#include "HeaderSubSystem.h"
#include "ManagerScenario.h"

using namespace nsMelissaSubSystem;

namespace nsMelissa
{
//-------------------------------------------------------------------------
TBase::TBase()
{
  mManagerSession = new TManagerSession;
  mManagerScenario = new TManagerScenario;
  mLoadProcent    = 0;
}
//-------------------------------------------------------------------------
TBase::~TBase()
{
  mManagerSession->GetCallbackRecv()->Unregister(this);
  mManagerSession->GetCallbackDisconnect()->Unregister(this);

  delete mManagerSession;
  mManagerSession = NULL;
  delete mManagerScenario;
  mManagerScenario = NULL;
}
//-------------------------------------------------------------------------
void TBase::Init(IMakerTransport* pMakerTransport)
{
  if(pMakerTransport==NULL)
  {
    GetLogger()->Get(STR_NAME_MELISSA)->WriteF_time("TBase::Init() pMakerTransport==NULL.\n");
    BL_FIX_BUG();
    return;
  }
  mManagerSession->SetMakerTransport(pMakerTransport);
  mManagerSession->GetCallbackRecv()->Register(&TBase::Recv, this);
  mManagerSession->GetCallbackDisconnect()->Register(&TBase::Disconnect, this);
}
//-------------------------------------------------------------------------
bool TBase::Open(unsigned short port, unsigned char subNet)
{
  return mManagerSession->Start(port, subNet);
}
//-------------------------------------------------------------------------
void TBase::SetLoad(int procent)
{
  mLoadProcent = procent;
}
//-------------------------------------------------------------------------
void TBase::DisconnectUp()
{

}
//-------------------------------------------------------------------------
void TBase::SendUp(TBreakPacket bp, bool check)
{

}
//-------------------------------------------------------------------------
bool TBase::IsConnectUp()
{
	return true;
}
//-------------------------------------------------------------------------
bool TBase::IsConnect(unsigned int id)
{
  return mManagerSession->IsExist(id);
}
//-------------------------------------------------------------------------
void TBase::Recv( TDescRecvSession* pDesc )
{
  TBaseHeader* pPacket = (TBaseHeader*)pDesc->data;

  switch(pPacket->from)
  {
    case eFromClient:
      RecvFromClient(pDesc);
      break;
    case eFromSlave:
      RecvFromSlave(pDesc);
      break;
    case eFromMaster:
      RecvFromMaster(pDesc);
      break;
    case eFromSuperServer:
      RecvFromSuperServer(pDesc);
      break;
    default:BL_FIX_BUG();
  }
}
//-------------------------------------------------------------------------
void TBase::RecvFromClient(TDescRecvSession* pDesc)
{
  BL_FIX_BUG();// если не переопределили, значит не ожидали
}
//-------------------------------------------------------------------------
void TBase::RecvFromSlave(TDescRecvSession* pDesc)
{
  BL_FIX_BUG();
}
//-------------------------------------------------------------------------
void TBase::RecvFromMaster(TDescRecvSession* pDesc)
{
  BL_FIX_BUG();
}
//-------------------------------------------------------------------------
void TBase::RecvFromSuperServer(TDescRecvSession* pDesc)
{
  BL_FIX_BUG();
}
//-------------------------------------------------------------------------
void TBase::Disconnect(unsigned int id)
{
  unsigned int* pID = new unsigned int(id);
  mIDSessionNeedDisconnect.Add(pID);
}
//-------------------------------------------------------------------------
void TBase::Work()
{
  //пробежка по ожидающим разъединения и удаление сессий
  mManagerSession->Work();

  WorkInherit();
}
//-------------------------------------------------------------------------
void TBase::SetTimeLiveSession(unsigned int time_ms)
{
  mManagerSession->SetTimeLiveSession(time_ms);
}
//-------------------------------------------------------------------------
}