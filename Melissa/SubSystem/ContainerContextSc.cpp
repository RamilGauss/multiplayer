/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ContainerContextSc.h"
#include <boost/foreach.hpp>

using namespace nsMelissa;

//------------------------------------------------------------
TContainerContextSc::TContainerContextSc()
{
  mManagerContextSc = NULL;

  mListContext.push_back(&mDisClient   );
  mListContext.push_back(&mDisSlave    );
  mListContext.push_back(&mFlow        );
  mListContext.push_back(&mLoginClient );
  mListContext.push_back(&mLoginSlave  );
  mListContext.push_back(&mLoginMaster );
  mListContext.push_back(&mRcm         );
  mListContext.push_back(&mSynchroSlave);
  mListContext.push_back(&mSendToClient);
}
//------------------------------------------------------------
void TContainerContextSc::SetID_Session(unsigned int id_session)
{
  BOOST_FOREACH(IContextScenario* p,mListContext)
    p->SetID_Session(id_session);
}
//------------------------------------------------------------
void TContainerContextSc::SetMCSc(TManagerContextSc* pMCSc)
{
  mManagerContextSc = pMCSc;

  BOOST_FOREACH(IContextScenario* p,mListContext)
    p->SetMSc(mManagerContextSc);
}
//------------------------------------------------------------
void TContainerContextSc::SetMS(TManagerSession* pMS)
{
  BOOST_FOREACH(IContextScenario* p,mListContext)
    p->SetMS(pMS);
}
//------------------------------------------------------------
void TContainerContextSc::SetSE(TSrcEvent* pSE)
{
  BOOST_FOREACH(IContextScenario* p,mListContext)
    p->SetSE(pSE);
}
//------------------------------------------------------------
void TContainerContextSc::SetUserPtr(void* p)
{
  BOOST_FOREACH(IContextScenario* p,mListContext)
    p->SetUserPtr(p);
}
//------------------------------------------------------------
