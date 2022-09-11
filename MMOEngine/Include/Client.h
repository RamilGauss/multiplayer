/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef CLIENT_H
#define CLIENT_H

#include "Base.h"

namespace nsMMOEngine
{
  class IScenario;
  class MMO_ENGINE_EI TClient : public TBase
  {
    
    unsigned int mID;// ����������� ��������
  public:
    TClient();
    virtual ~TClient();

    void Login(unsigned int ip, unsigned short port, void* data, int size);
    void LeaveQueue();
	protected:
    // Base
    virtual void DisconnectInherit(unsigned int id);

    virtual void SetIDClient(unsigned int id_client);
	private:
    virtual void EndLoginClient(IScenario*);
    virtual void EndRcm(IScenario*);

  };
}

#endif