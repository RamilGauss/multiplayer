/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_EVENTS_H
#define MELISSA_EVENTS_H

#include "BaseEvent.h"
#include "Master.h"

namespace nsMelissa
{
#if defined( WIN32 )
#pragma pack(push, 1)
#endif

  struct MELISSA_EI TEventRecvFromDown : public TBaseEvent
  {
    TEventRecvFromDown();
    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventRecvFromUp : public TBaseEvent
  {
    TEventRecvFromUp();
    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventCreateGroup : public TBaseEvent
  {
    TEventCreateGroup();
    unsigned int id_group;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventDisconnectUp : public TBaseEvent
  {
    TEventDisconnectUp();
    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventDisconnectDown : public TBaseEvent
  {
    TEventDisconnectDown();
    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventConnectUp : public TBaseEvent
  {
    TEventConnectUp();
    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventConnectDown : public TBaseEvent
  {
    TEventConnectDown();
    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventLeaveGroup: public TBaseEvent
  {
    TEventLeaveGroup();
    unsigned int id_client;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventDestroyGroup : public TBaseEvent
  {
    TEventDestroyGroup();
    unsigned int id_group;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventResultLogin : public TBaseEvent
  {
    TEventResultLogin();
    TMaster::tResultLogin res;
    int numInQueue;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventError : public TBaseEvent
  {
    TEventError();
    unsigned int code;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventRestoreContext : public TBaseEvent
  {
    TEventRestoreContext();
    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventTryLogin : public TBaseEvent
  {
    TEventTryLogin();
    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventSaveContext : public TBaseEvent
  {
    TEventSaveContext();
    unsigned int id_session;
  };
  //-------------------------------------------------------------
#if defined( WIN32 )
#pragma pack(pop)
#endif

}
#endif