/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CONTAINER_CONTEXT_SC_H
#define MELISSA_CONTAINER_CONTEXT_SC_H

#include "ManagerContextSc.h"
#include <list>

#include "ContextScDisconnectClient.h"
#include "ContextScDisconnectSlave.h"
#include "ContextScFlow.h"
#include "ContextScLoginClient.h"
#include "ContextScLoginSlave.h"
#include "ContextScLoginMaster.h"
#include "ContextScRecommutationClient.h"
#include "ContextScSynchroSlave.h"
#include "ContextScSendToClient.h"

namespace nsMelissa
{
  // ��������, ������������ � ���������� ������� �����������
  class TContainerContextSc
	{
    typedef std::list<IContextScenario*> TListPtr;
    TListPtr mListContext;
  
  protected:
    TManagerContextSc* mManagerContextSc;
	public:
    TContextScDisconnectClient    mDisClient;
    TContextScDisconnectSlave     mDisSlave;
    TContextScFlow                mFlow;
    TContextScLoginClient         mLoginClient;
    TContextScLoginSlave          mLoginSlave;
    TContextScLoginMaster         mLoginMaster;
    TContextScRecommutationClient mRcm;
    TContextScSynchroSlave        mSynchroSlave;
    TContextScSendToClient        mSendToClient;

		TContainerContextSc();

    void SetMCSc(TManagerContextSc* pMCSc);
    void SetID_Session(unsigned int id_session);
    void SetMS(TManagerSession* pMS);
    void SetSE(TSrcEvent* pSE);
    void SetUserPtr(void* p);
	};
}  
#endif