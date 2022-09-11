/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
√удаков –амиль —ергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_BASESERVER_H
#define MELISSA_BASESERVER_H

#include "Base.h"

namespace nsMelissa
{
  class MELISSA_EI TBaseServer : public TBase
  {

  public:
    TBaseServer();
    virtual ~TBaseServer();
    
		// всегда с гарантией доставки, т.к. тогда не имеет смысла так долго пересылать (слишком много затрат сил, а гарантии нет?)
    virtual void SendByClientKey(std::list<unsigned int>& l, TBreakPacket bp) = 0;

    virtual void SendDown(unsigned int id_session, TBreakPacket bp, bool check = true) = 0;
    virtual int  GetCountDown() = 0;
    virtual bool GetDescDown(int index, void* pDesc, int& sizeDesc) = 0;

	protected:

	private:

  };
}

#endif