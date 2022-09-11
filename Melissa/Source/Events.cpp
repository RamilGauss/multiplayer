/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Events.h"
#include "ManagerSession.h"

namespace nsMelissa
{

TEventRecvFromDown::TEventRecvFromDown()
{
  mType = TBase::eRecvFromDown;
  id_session = INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------
TEventRecvFromUp::TEventRecvFromUp()
{
  mType = TBase::eRecvFromUp;
  id_session = INVALID_HANDLE_SESSION;
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
}
//-------------------------------------------------------------
TEventTryLogin::TEventTryLogin()
{
  mType = TBase::eTryLogin;
  id_session = INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------
TEventSaveContext::TEventSaveContext()
{
  mType = TBase::eSaveContext;
  id_session = INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------

}
