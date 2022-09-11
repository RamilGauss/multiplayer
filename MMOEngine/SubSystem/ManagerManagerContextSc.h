/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MANAGER_MANAGER_CONTEXT_SC_H
#define MANAGER_MANAGER_CONTEXT_SC_H

#include <set>
#include "ManagerContextSc.h"

namespace nsMMOEngine
{
  class TManagerManagerContextSc
  {
    typedef std::set<TManagerContextSc*> TSetPtr;
    TSetPtr mSetManagerContextSc;

    TSetPtr mSetActiveManagerContextSc;
  public:
    TManagerManagerContextSc();
    ~TManagerManagerContextSc();

    TManagerContextSc* Add();
    void Remove(TManagerContextSc* pM�Sc);
    // ��� ��������� ���������� �������
    void Work();
  protected:
    void ActiveEvent(TManagerContextSc* pMCSc);
    void DisactiveEvent(TManagerContextSc* pMCSc);
  };
}

#endif