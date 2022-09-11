/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_CLIENT_H
#define MELISSA_CLIENT_H

#include "Base.h"

namespace nsMelissa
{
  class IScenario;
  class MELISSA_EI TClient : public TBase
  {
  public:

    TClient();
    virtual ~TClient();

    void Login(unsigned int ip, unsigned short port, void* data, int size);
    void LeaveQueue();
	protected:
    // Base
    virtual void DisconnectInherit(unsigned int id);
	private:
    virtual void EndLoginClient(IScenario*);
    virtual void EndRcm(IScenario*);

  };
}

#endif