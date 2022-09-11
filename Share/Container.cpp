/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "Container.h"

#include <memory.h>
#include <stddef.h>

TContainer::TContainer()
{
  pData = NULL;
  size  = 0;
}
//---------------------------------------------------------------
TContainer::~TContainer()
{
  Done();
}
//---------------------------------------------------------------
void TContainer::Done()
{
  delete[]pData;
  pData = NULL;
  size  = 0;
}
//---------------------------------------------------------------
void TContainer::SetData(char* p, int s)
{
  Done();
  size  = s;
  pData = new char[size];
  if(p)
    memcpy(pData,p,size);
}
//---------------------------------------------------------------
char* TContainer::GetData(int &s)
{
  s = size;
  return pData;
}
//---------------------------------------------------------------
void* TContainer::GetPtr()const
{
  return pData;
}
//---------------------------------------------------------------
int TContainer::GetSize()const
{
  return size;
}
//---------------------------------------------------------------
void TContainer::Unlink()
{
  pData = NULL;
  size = 0;
}
//---------------------------------------------------------------
void TContainer::Entrust(char* p, int s)
{
  Done();
  size  = s;
  pData = p;
}
//---------------------------------------------------------------
