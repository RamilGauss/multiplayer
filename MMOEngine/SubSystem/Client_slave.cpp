/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Client_slave.h"

using namespace nsMMOEngine;

TClient_slave::TClient_slave()
{

}  
//----------------------------------------------------------------------
TContainerContextSc* TClient_slave::GetC()
{
  return &mC;
}
//----------------------------------------------------------------------
void TClient_slave::SetID_SessionDown(unsigned int v)
{
  mID_SessionDown = v;
}
//----------------------------------------------------------------------
unsigned int TClient_slave::GetID_SessionDown()
{
  return mID_SessionDown;
}
//----------------------------------------------------------------------
void TClient_slave::SetKey(unsigned int v)
{
  mClientKey = v;
}
//----------------------------------------------------------------------
unsigned int TClient_slave::GetKey()
{
  return mClientKey;
}
//----------------------------------------------------------------------
