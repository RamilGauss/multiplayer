/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MELISSA_MANAGER_CONTEXT_SC_H
#define MELISSA_MANAGER_CONTEXT_SC_H

#include <list>
#include "CallBackRegistrator.h"

namespace nsMelissa
{
  class IContextScenario;
  class TManagerContextSc
  {
    typedef std::list<IContextScenario*> TListPtr;
    typedef TListPtr::iterator TListPtrIt;

    TListPtr   mListWaitActivation;
    IContextScenario* pActiveContextSc;// �������� �� ������ ������ 

    TCallBackRegistrator1<TManagerContextSc*> mCallBackActivateEvent;
    TCallBackRegistrator1<TManagerContextSc*> mCallBackDisactivateEvent;
  public:
    TManagerContextSc();
    ~TManagerContextSc();

    // ��� ��������� ���������� �������
    void Work();

    bool Activate(IContextScenario* pCSc);
    void Disactivate();

  protected:
    friend class TManagerManagerContextSc;

    bool IsActive();
    TCallBackRegistrator1<TManagerContextSc*>* GetCallBackActivate();
    TCallBackRegistrator1<TManagerContextSc*>* GetCallBackDisactivate();
  private:
    void NotifyActiveEvent();
    void NotifyDisactiveEvent();
  };
}

#endif