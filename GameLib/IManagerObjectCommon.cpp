/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "IManagerObjectCommon.h"
#include "BL_Debug.h"

IManagerObjectCommon::IManagerObjectCommon()
{

}
//--------------------------------------------------------
IManagerObjectCommon::~IManagerObjectCommon()
{

}
//--------------------------------------------------------
void IManagerObjectCommon::AddObject(IBaseObjectCommon* pObject)
{
  mVectorObject.push_back(pObject);
}
//--------------------------------------------------------
// ������ ������ �� ��������� ������� �������
IBaseObjectCommon* IManagerObjectCommon::Get(int index)
{
  BL_ASSERT(mVectorObject.size() > (unsigned int)index);
  IBaseObjectCommon* pObject = mVectorObject.at(index);
  return pObject;
}
//--------------------------------------------------------
int IManagerObjectCommon::GetCountObject()
{
  return mVectorObject.size();
}
//--------------------------------------------------------
