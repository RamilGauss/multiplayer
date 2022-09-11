/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ACTIVESERVER_H
#define ACTIVESERVER_H

#include "BaseServer.h"

namespace nsMMOEngine
{
  class MMO_ENGINE_EI TActiveServer : public TBaseServer
  {

  public:
    TActiveServer();
    virtual ~TActiveServer();
    
    virtual void ConnectUp(unsigned int ip, unsigned short port) = 0;

	protected:

	private:

  };
}

#endif