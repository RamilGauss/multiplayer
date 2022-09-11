/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_ACTIVESERVER_H
#define MELISSA_ACTIVESERVER_H

#include "BaseServer.h"

namespace nsMelissa
{
  class MELISSA_EI TActiveServer : public TBaseServer
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