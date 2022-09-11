/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ClientPrivate.h"

using namespace nsMelissa;

TClientPrivate::TClientPrivate()
{

}  
//----------------------------------------------------------------------
TContainerContextSc* TClientPrivate::GetC()
{
  return &mC;
}
//----------------------------------------------------------------------
void TClientPrivate::SetID_SessionDown(unsigned int v)
{
  mID_SessionDown = v;
}
//----------------------------------------------------------------------
unsigned int TClientPrivate::GetID_SessionDown()
{
  return mID_SessionDown;
}
//----------------------------------------------------------------------
void TClientPrivate::SetKey(unsigned int v)
{
  mClientKey = v;
}
//----------------------------------------------------------------------
unsigned int TClientPrivate::GetKey()
{
  return mClientKey;
}
//----------------------------------------------------------------------
