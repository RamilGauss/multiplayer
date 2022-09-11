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

#include "Events.h"
#include "ManagerSession.h"

namespace nsMelissa
{

TEventRecvFromDown::TEventRecvFromDown()
{
  mType = TBase::eRecvFromDown;
  id_session = INVALID_HANDLE_SESSION;
  sizeData = 0;
}
//-------------------------------------------------------------
TEventRecvFromUp::TEventRecvFromUp()
{
  mType = TBase::eRecvFromUp;
  id_session = INVALID_HANDLE_SESSION;
  sizeData = 0;
}
//-------------------------------------------------------------
TEventCreateGroup::TEventCreateGroup()
{
  mType = TBase::eCreateGroup;
  id_group = 0;
}
//-------------------------------------------------------------
TEventDisconnectUp::TEventDisconnectUp()
{
  mType = TBase::eDisconnectUp;
  id_session = INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------
TEventDisconnectDown::TEventDisconnectDown()
{
  mType = TBase::eDisconnectDown;
  id_session = INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------
TEventConnectUp::TEventConnectUp()
{
  mType = TBase::eConnectUp;
  id_session = INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------
TEventConnectDown::TEventConnectDown()
{
  mType = TBase::eConnectDown;
  id_session = INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------
TEventLeaveGroup::TEventLeaveGroup()
{
  mType = TBase::eLeaveGroup;
  id_client = 0;
}
//-------------------------------------------------------------
TEventDestroyGroup::TEventDestroyGroup()
{
  mType = TBase::eDestroyGroup;
  id_group = 0;
}
//-------------------------------------------------------------
TEventResultLogin::TEventResultLogin()
{
  mType = TBase::eResultLogin;
  numInQueue = 0;
  sizeData   = 0;
}
//-------------------------------------------------------------
TEventError::TEventError()
{
  mType = TBase::eError;
  code = 0;
}
//-------------------------------------------------------------
TEventRestoreContext::TEventRestoreContext()
{
  mType = TBase::eRestoreContext;
  id_session = INVALID_HANDLE_SESSION;
  sizeData = 0;
}
//-------------------------------------------------------------
TEventTryLogin::TEventTryLogin()
{
  mType = TBase::eTryLogin;
  id_session = INVALID_HANDLE_SESSION;
  sizeData = 0;
}
//-------------------------------------------------------------
TEventSaveContext::TEventSaveContext()
{
  mType = TBase::eSaveContext;
  id_session = INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------

}
