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

#ifndef MELISSA_EVENTS_H
#define MELISSA_EVENTS_H

#include "BaseEvent.h"

namespace nsMelissa
{
#define GET_PTR_AFTER_HEADER \
  char* GetPtrAfterHeader(){return (char*)(this + 1);}

#if defined( WIN32 )
#pragma pack(push, 1)
#endif

  struct MELISSA_EI TEventRecvFromDown : public TBaseEvent
  {
    TEventRecvFromDown();
    GET_PTR_AFTER_HEADER

    unsigned int id_session;
    int       sizeData;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventRecvFromUp : public TBaseEvent
  {
    TEventRecvFromUp();
    GET_PTR_AFTER_HEADER

    unsigned int id_session;
    int       sizeData;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventCreateGroup : public TBaseEvent
  {
    TEventCreateGroup();
    GET_PTR_AFTER_HEADER
    
    unsigned int id_group;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventDisconnectUp : public TBaseEvent
  {
    TEventDisconnectUp();
    GET_PTR_AFTER_HEADER

    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventDisconnectDown : public TBaseEvent
  {
    TEventDisconnectDown();
    GET_PTR_AFTER_HEADER

    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventConnectUp : public TBaseEvent
  {
    TEventConnectUp();
    GET_PTR_AFTER_HEADER
  
    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventConnectDown : public TBaseEvent
  {
    TEventConnectDown();
    GET_PTR_AFTER_HEADER

    unsigned int id_session;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventLeaveGroup: public TBaseEvent
  {
    TEventLeaveGroup();
    GET_PTR_AFTER_HEADER

    unsigned int id_client;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventDestroyGroup : public TBaseEvent
  {
    TEventDestroyGroup();
    GET_PTR_AFTER_HEADER
    
    unsigned int id_group;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventResultLogin : public TBaseEvent
  {
    TEventResultLogin();
    GET_PTR_AFTER_HEADER

    TBase::tResultLogin res;
    int numInQueue;
    int sizeData;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventError : public TBaseEvent
  {
    TEventError();
    GET_PTR_AFTER_HEADER

    unsigned int code;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventRestoreContext : public TBaseEvent
  {
    TEventRestoreContext();
    GET_PTR_AFTER_HEADER

    unsigned int id_session;
    int sizeData;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventTryLogin : public TBaseEvent
  {
    TEventTryLogin();
    GET_PTR_AFTER_HEADER

    unsigned int id_session;
    int sizeData;
  };
  //-------------------------------------------------------------
  struct MELISSA_EI TEventSaveContext : public TBaseEvent
  {
    TEventSaveContext();
    GET_PTR_AFTER_HEADER
      
    unsigned int id_session;
  };
  //-------------------------------------------------------------
#if defined( WIN32 )
#pragma pack(pop)
#endif

}
#endif