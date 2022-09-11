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

#include "NavigateSession.h"
#include "Session.h"
#include "Logger.h"
#include "Base.h"

using namespace std;

namespace nsMelissa
{
TNavigateSession::TNavigateSession()
{

}
//--------------------------------------------------------------
TNavigateSession::~TNavigateSession()
{
  TSetPtrIt bit = mSetSession.begin();
  TSetPtrIt eit = mSetSession.end();
  while(bit!=eit)
  {
    delete *bit;
    bit++;
  }
}
//--------------------------------------------------------------
void TNavigateSession::Add(TSession* pSession)
{
  TIP_Port ip_port;
  pSession->GetInfo(ip_port);
  mMapID_Session.insert(TMapUintPtrVT(pSession->GetID(), pSession));
  mMapIP_Session.insert(TMapIP_PtrItVT(ip_port,          pSession));
  mSetSession.insert(pSession);
}
//--------------------------------------------------------------
void TNavigateSession::Delete(TSession* pSession)
{
  TIP_Port ip_port;
  pSession->GetInfo(ip_port);
  mMapIP_Session.erase(ip_port);
  mMapID_Session.erase(pSession->GetID());
  mSetSession.erase(pSession);
  delete pSession;
}
//--------------------------------------------------------------
TSession* TNavigateSession::FindSessionByIP(TIP_Port& ip_port)
{
  TSession* pSession = NULL;
  TMapIP_PtrIt fit = mMapIP_Session.find(ip_port);
  if(fit!=mMapIP_Session.end())
    pSession = fit->second;
  return pSession;
}
//--------------------------------------------------------------
TSession* TNavigateSession::FindSessionByID(unsigned int id)
{
  TSession* pSession = NULL;
  TMapUintPtrIt fit = mMapID_Session.find(id);
  if(fit!=mMapID_Session.end())
    pSession = fit->second;
  return pSession;
}
//--------------------------------------------------------------
bool TNavigateSession::IsExist(TSession* pSession)
{
  return (mSetSession.find(pSession)!=mSetSession.end());
}
//--------------------------------------------------------------
set<TSession*>::iterator TNavigateSession::Begin()
{
  return mSetSession.begin();
}
//--------------------------------------------------------------
set<TSession*>::iterator TNavigateSession::End()
{
  return mSetSession.end();
}
//--------------------------------------------------------------

}

