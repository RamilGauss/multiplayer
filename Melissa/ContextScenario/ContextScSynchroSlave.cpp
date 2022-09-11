/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ContextScSynchroSlave.h"

using namespace nsMelissa;

TContextScSynchroSlave::TContextScSynchroSlave()
{
  mLoadProcent = 0;
}
//------------------------------------------------------------------
TContextScSynchroSlave::~TContextScSynchroSlave()
{

}
//------------------------------------------------------------------    
void TContextScSynchroSlave::SetLoadProcent(unsigned char v)
{
  mLoadProcent = v;
}
//------------------------------------------------------------------    
unsigned char TContextScSynchroSlave::GetLoadProcent()
{
  return mLoadProcent;
}
//------------------------------------------------------------------    
