/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#include "MarkUpContainer.h"
#include "memory_operation.h"

using namespace std;

//--------------------------------------------------------------------
TMarkUpContainer::TMarkUpContainer()
{
	SetSizeAppendix(0);
}
//--------------------------------------------------------------------
TMarkUpContainer::~TMarkUpContainer()
{
  Done();
}
//--------------------------------------------------------------------
void TMarkUpContainer::SetMarkUp(vector<TCommonDesc>* pVecSize)
{
  mVectorSection.clear();
  mMarkUp.clear();
  int cnt = pVecSize->size();
  int shift = 0;
  for(int i = 0 ; i < cnt ; i++ )
  {
    TDesc_Private desc_p;
    desc_p.c = pVecSize->operator[](i);
    desc_p.shift = shift;

    mVectorSection.push_back(desc_p);

    mMarkUp.insert(TMapStrInt::value_type(desc_p.c.name,i));

    shift += GetSize(desc_p, NULL);
  }
  mC.SetData(NULL,shift);

  ZeroTensileField();
}
//--------------------------------------------------------------------
void* TMarkUpContainer::Get(const char* name, bool type_counter, int index)
{
  TMapStrInt::iterator fit = mMarkUp.find(name);
  if(fit==mMarkUp.end())
    return NULL;

  char* ptr = (char*)GetPtr();

  TDesc_Private& desc = mVectorSection.operator[](fit->second);
  ptr += desc.shift;

  switch(desc.c.type)
  {
    case eVar:
      if(type_counter==false)
      {
        // ��������
        int cnt = GetValueBy(ptr,desc.c.v.sizeCnt);
        if(index>=cnt) return NULL;// �������

        ptr += desc.c.v.sizeCnt;
        ptr += index*desc.c.v.sizeVar;
      }
      break;
    case eMarkUp:
      if(type_counter==false)
        ptr += desc.c.m.sizeSize;
      break;
    default:;
  }
  return ptr;
}
//--------------------------------------------------------------------
void* TMarkUpContainer::GetPtr() const
{
  return mC.GetPtr();
}
//--------------------------------------------------------------------
int TMarkUpContainer::GetSize()
{
  return mC.GetSize();
}
//--------------------------------------------------------------------
bool TMarkUpContainer::Set(void* p, int size)
{
  // �����, �� ������� ����������� ��������� �� ������ p ��������
  // ��� ���������� ������ ���-�� ��������� � ����� ���� eVar
  // ���� - ��������� ���� shift, cntVar 
  mC.SetData((char*)p,size);

  char* ptr = (char*)GetPtr();
  int shift = 0;
  int cnt = mVectorSection.size();
  for( int i = 0 ; i < cnt ; i++)
  {
    TDesc_Private& desc_p = mVectorSection.operator[](i);// ����� �������� ������

    int sizeSection = GetSize(desc_p, ptr);// ���������� ������ ������

    switch(desc_p.c.type)
    {
      case eVar:
        desc_p.cntVar = GetValueBy(ptr,desc_p.c.v.sizeCnt);
        break;
      case eMarkUp:
        desc_p.sizeMarkUp = GetValueBy(ptr,desc_p.c.m.sizeSize);
        break;
      default:;
    }

    desc_p.shift  = shift;

    shift += sizeSection; // 
    ptr   += sizeSection;
  }
  if(shift!=mC.GetSize())
    return false;
  return true;
}
//--------------------------------------------------------------------
void TMarkUpContainer::Done()
{
	SetSizeAppendix(0);
  mC.Done();

  mMarkUp.clear();
  mVectorSection.clear();
}
//--------------------------------------------------------------------
void TMarkUpContainer::Update()
{
  vector<int> vecDisVar;// ������ ��������������
  // ������� ���-�� �������������� �� �����
  int cntSection = mVectorSection.size();
  for(int i = 0 ; i < cntSection ; i++ )
  {
    TDesc_Private& desc_p = mVectorSection.operator[](i);

    switch(desc_p.c.type)
    {
      case eVar:
      {
        char* ptr = (char*)GetPtr() + desc_p.shift;
        int freshCnt = GetValueBy(ptr,desc_p.c.v.sizeCnt);
        if(desc_p.cntVar!=freshCnt)
					vecDisVar.push_back(i);
      }
        break;
      case eMarkUp:
      {
        char* ptr = (char*)GetPtr() + desc_p.shift;
        int freshSize = GetValueBy(ptr, desc_p.c.v.sizeCnt);
        if(desc_p.sizeMarkUp!=freshSize)
					vecDisVar.push_back(i);
      }
        break;
      default:;
    }
  }
  //--------------------------------------------------------
  // ��������� ������ ��������������
  // ������� ���-�� �������������� �� ���� eVar � eMarkUp
  int cntDisVar = vecDisVar.size();
  for(int d = 0 ; d < cntDisVar ; d++ )
  {
    int i = vecDisVar.operator[](d);

    TDesc_Private& desc_p = mVectorSection.operator[](i);

    char* ptr = (char*)GetPtr() + desc_p.shift;          // ��������� �� ������ ������ (�������� � ������ ���-�� ��������� ������, ���-�� ������� ����������)
    int freshValue = GetValueBy(ptr,desc_p.c.v.sizeCnt); // ��������� ���-�� �� ������
    //------------------------------------------------------------------------------
    int d_size = 0;// �������� �� �������
    // ������ ������� � ������� � ��������� ������ �������� �������
    switch(desc_p.c.type)
    {
      case eVar:
        d_size = desc_p.c.v.sizeVar*(desc_p.cntVar - freshValue);
        desc_p.cntVar = freshValue;
        break;
      case eMarkUp:
        d_size = desc_p.sizeMarkUp - freshValue;
        desc_p.sizeMarkUp = freshValue;
        break;
      default:;
    }
    //-----------------------------------------------------------------------------
    int newSize  = GetSize() - d_size;        // ����� ������

    void* newPtr = GetPtr();
    if(d_size < 0)
    {
      int sizeBound = desc_p.shift + GetSizeByDesc(desc_p) + d_size;

      // ���������� �������
      newPtr = mo_realloc_bound_new((char*)GetPtr(), GetSize(), sizeBound, -d_size);
    }
    else
    {
      int sizeBound = desc_p.shift + GetSizeByDesc(desc_p);
      // ���������� �������
      char* ptrBoundLow = (char*)GetPtr() + sizeBound;
      int sizeMove      = GetSize() - sizeBound - d_size;
      memmove(ptrBoundLow, ptrBoundLow + d_size, sizeMove);
    }
    mC.Unlink();
    mC.SetData((char*)newPtr,newSize);
    delete[](char*)newPtr;
    
    // �������� ������ ��� ������� �����
    for(int j = i+1 ; j < cntSection ; j++ )
    {
      mVectorSection.operator[](j).shift -= d_size;    
    }
  }
}
//--------------------------------------------------------------------
int TMarkUpContainer::GetValueBy(char* ptr, int sizeCnt)
{
  int res = 0;
  switch(sizeCnt)
  {
    case sizeof(char):
      res = *ptr;
      break;
    case sizeof(short):
      res = *((short*)ptr);
      break;
    case sizeof(int):
      res = *((int*)ptr);
      break;
    default:;
  }
  return res;
}
//--------------------------------------------------------------------
void TMarkUpContainer::SetCountBy(char* ptr, int sizeCnt, int v)
{
  switch(sizeCnt)
  {
    case 1:
      *ptr = (char)v;
      break;
    case 2:
      *(short*)ptr = (short)v;
      break;
    case 4:
      *(int*)ptr = v;
      break;
    default:;
  }
}
//--------------------------------------------------------------------
int TMarkUpContainer::GetSize(TDesc_Private& desc_p, char* ptr)
{
  int res = 0;
  switch(desc_p.c.type)
  {
    case eConst:
      res = desc_p.c.c.size;
      break;
    case eVar:
      res  = desc_p.c.v.sizeCnt;
      if(ptr!=NULL)// ��-��������� ����� 0
        res += desc_p.c.v.sizeVar*GetValueBy(ptr,desc_p.c.v.sizeCnt);
      break;
    case eMarkUp:
      res = desc_p.c.m.sizeSize;
      if(ptr!=NULL)// ��-��������� ����� 0
        res += GetValueBy(ptr,desc_p.c.m.sizeSize);
      break;
    default:;
  }

  return res;
}
//--------------------------------------------------------------------
void TMarkUpContainer::ZeroTensileField()
{
  char* ptr = (char*)GetPtr(); 
  if(ptr==NULL) return;

  int cnt = mVectorSection.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    TDesc_Private& desc_p = mVectorSection.operator[](i);
    switch(desc_p.c.type)
    {
      case eVar:
        SetCountBy(ptr+desc_p.shift, desc_p.c.v.sizeCnt, 0);
        break;
      case eMarkUp:
        SetCountBy(ptr+desc_p.shift, desc_p.c.m.sizeSize, 0);
        break;
      default:;
    }
  }
}
//--------------------------------------------------------------------
int TMarkUpContainer::GetSizeByDesc(TDesc_Private& desc_p)
{
  int res = 0;
  switch(desc_p.c.type)
  {
    case eVar:
      res = desc_p.c.v.sizeCnt + desc_p.c.v.sizeVar * desc_p.cntVar;
      break;
    case eMarkUp:
      res = desc_p.sizeMarkUp + desc_p.c.m.sizeSize;
      break;
    default:;
  }
  return res;
}
//--------------------------------------------------------------------
void TMarkUpContainer::SetDataAppendix(void* p, int size)
{
	if(mSizeAppendix!=size)
	{
		int newSize = GetSize() + size - GetSizeAppendix();
		void* newPtr = mo_realloc_new(GetPtr(), GetSize(), newSize);
		SetSizeAppendix(size);

		mC.Unlink();
		mC.SetData((char*)newPtr, newSize);
		delete[](char*)newPtr;
	}

	void* pBeginAppendix = GetPtrAppendix();
	memcpy(pBeginAppendix, p, GetSizeAppendix());
}
//--------------------------------------------------------------------
void* TMarkUpContainer::GetDataAppendix(int& size)
{
	size = GetSizeAppendix();
	return GetPtrAppendix();
}
//--------------------------------------------------------------------
int TMarkUpContainer::GetSizeAppendix()
{
	return mSizeAppendix;
}
//--------------------------------------------------------------------
void TMarkUpContainer::SetSizeAppendix(int v)
{
	mSizeAppendix = v;
}
//--------------------------------------------------------------------
void* TMarkUpContainer::GetPtrAppendix()
{
	int sizeWithoutAppendix = GetSize() - GetSizeAppendix();
	return (char*)GetPtr() + sizeWithoutAppendix;
}
//--------------------------------------------------------------------
