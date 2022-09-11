/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CONTEXT_SC_LOGIN_CLIENT_H
#define MELISSA_CONTEXT_SC_LOGIN_CLIENT_H

#include "IContextScenario.h"
#include "BreakPacket.h"

namespace nsMelissa
{
  // сценарии, содержащиеся в контейнере взаимно блокируются
  class TContextScLoginClient : public IContextScenario
	{
    typedef enum
    {
      eUndef,
      eReject,
      eAccept,
    }tStateResult;
    tStateResult mState;

    unsigned int mTimeWaitAnswer;

	public:
		TContextScLoginClient();
    virtual ~TContextScLoginClient();

    void SetTimeWait(unsigned int v);
    unsigned int GetTimeWait();

    void Undef();
    void Accept();
    void Reject();
    
    bool IsReject();
  private:
	};
}  
#endif