/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef MANAGER_CONTEXT_SC_H
#define MANAGER_CONTEXT_SC_H

#include <list>
#include "CallBackRegistrator.h"

namespace nsMMOEngine
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

    IContextScenario* GetActive();

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