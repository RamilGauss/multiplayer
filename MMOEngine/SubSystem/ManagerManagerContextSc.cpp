/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ManagerManagerContextSc.h"
#include <boost/foreach.hpp>

using namespace nsMMOEngine;

TManagerManagerContextSc::TManagerManagerContextSc()
{

}
//-----------------------------------------------------------------------------
TManagerManagerContextSc::~TManagerManagerContextSc()
{
  BOOST_FOREACH( TManagerContextSc* pMSc, mSetManagerContextSc )
    delete pMSc;
  mSetManagerContextSc.clear();
}
//-----------------------------------------------------------------------------
TManagerContextSc* TManagerManagerContextSc::Add()
{
  TManagerContextSc* pM�Sc = new TManagerContextSc();
  pM�Sc->GetCallBackActivate()   ->Register(&TManagerManagerContextSc::ActiveEvent,this);
  pM�Sc->GetCallBackDisactivate()->Register(&TManagerManagerContextSc::DisactiveEvent,this);
  mSetManagerContextSc.insert(pM�Sc);
  return pM�Sc;
}
//-----------------------------------------------------------------------------
void TManagerManagerContextSc::Remove(TManagerContextSc* pM�Sc)
{
  mSetManagerContextSc.erase(pM�Sc);
  mSetActiveManagerContextSc.erase(pM�Sc);
  delete pM�Sc;
}
//-----------------------------------------------------------------------------
void TManagerManagerContextSc::Work()
{
  BOOST_FOREACH( TManagerContextSc* pMSc, mSetActiveManagerContextSc )
    pMSc->Work();// �������� ������ �������� ��������
}
//-----------------------------------------------------------------------------
void TManagerManagerContextSc::ActiveEvent(TManagerContextSc* pMCSc)
{
  mSetActiveManagerContextSc.insert(pMCSc);
}
//-----------------------------------------------------------------------------
void TManagerManagerContextSc::DisactiveEvent(TManagerContextSc* pMCSc)
{
  if(pMCSc->IsActive()==false)
    mSetActiveManagerContextSc.erase(pMCSc);
}
//-----------------------------------------------------------------------------
