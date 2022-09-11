/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ManagerContextSc.h"
#include "IScenario.h"
#include "BL_Debug.h"
#include "IContextScenario.h"


using namespace std;
using namespace nsMelissa;

TManagerContextSc::TManagerContextSc()
{
  pActiveContextSc = NULL;
}
//---------------------------------------------------------------------
TManagerContextSc::~TManagerContextSc()
{

}
//---------------------------------------------------------------------
bool TManagerContextSc::Activate(IContextScenario* pCSc)
{
  bool res;
  if(pActiveContextSc)// ���� ���� ��������, �� ��������� � ������� �� ���������
  {
    // ���� ���� ������ ������� ��� �� ��������, ��������� � �������
    mListWaitActivation.push_back(pCSc);
    res = false;
  }
  else
  {
    pActiveContextSc = pCSc;
    res = true;
  }
  
  NotifyActiveEvent();
  return res;
}
//---------------------------------------------------------------------
void TManagerContextSc::Disactivate()
{
  BL_ASSERT(pActiveContextSc);
  // ��������� ��������
  if(mListWaitActivation.size())
  {
    pActiveContextSc = mListWaitActivation.front();
    mListWaitActivation.pop_front();

    pActiveContextSc->DelayBegin();
  }
  else
    pActiveContextSc = NULL;

  NotifyDisactiveEvent();
}
//---------------------------------------------------------------------
void TManagerContextSc::Work()
{
  if(pActiveContextSc)
    pActiveContextSc->Work();
}
//---------------------------------------------------------------------
bool TManagerContextSc::IsActive()
{
  return pActiveContextSc!=NULL;
}
//---------------------------------------------------------------------
TCallBackRegistrator1<TManagerContextSc*>* TManagerContextSc::GetCallBackActivate()
{
  return &mCallBackActivateEvent;
}
//---------------------------------------------------------------------
TCallBackRegistrator1<TManagerContextSc*>* TManagerContextSc::GetCallBackDisactivate()
{
  return &mCallBackDisactivateEvent;
}
//---------------------------------------------------------------------
void TManagerContextSc::NotifyActiveEvent()
{
  mCallBackActivateEvent.Notify(this);
}
//---------------------------------------------------------------------
void TManagerContextSc::NotifyDisactiveEvent()
{
  mCallBackDisactivateEvent.Notify(this);
}
//---------------------------------------------------------------------

