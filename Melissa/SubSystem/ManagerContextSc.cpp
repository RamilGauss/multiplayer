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
  if(pActiveContextSc)// ���� ���� ��������, �� ��������� � ������� �� ���������
  {
    // ���� ���� ������ ������� ��� �� ��������, ��������� � �������
    mListWaitActivation.push_back(pCSc);
    return false;
  }
  pActiveContextSc = pCSc;
  return true;
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
}
//---------------------------------------------------------------------
void TManagerContextSc::Work()
{
  if(pActiveContextSc)
    pActiveContextSc->Work();
}
//---------------------------------------------------------------------
