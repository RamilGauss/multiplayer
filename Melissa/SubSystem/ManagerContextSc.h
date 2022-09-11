/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_MANAGER_CONTEXT_SC_H
#define MELISSA_MANAGER_CONTEXT_SC_H

#include <list>

namespace nsMelissa
{
  class IContextScenario;
  class TManagerContextSc
  {
    typedef std::list<IContextScenario*> TListPtr;
    typedef TListPtr::iterator TListPtrIt;

    TListPtr   mListWaitActivation;
    IContextScenario* pActiveContextSc;// �������� �� ������ ������ 
  public:
    TManagerContextSc();
    ~TManagerContextSc();

    // ��� ��������� ���������� �������
    void Work();

    bool Activate(IContextScenario* pCSc);
    void Disactivate();
  };
}

#endif