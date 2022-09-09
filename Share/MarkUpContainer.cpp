/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "MarkUpContainer.h"
#include "memory_operation.h"

using namespace std;

#ifdef WIN32
  #define AT(i) at(i)
#else
  // ��� �������� ���� ������ �� ������������� at()
  #define AT(i) operator[](i)
#endif


TMarkUpContainer::TMarkUpContainer()
{

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
    desc_p.c = pVecSize->AT(i);
    desc_p.shift = shift;

    mVectorSection.push_back(desc_p);

    mMarkUp.insert(TMapStrInt::value_type(desc_p.c.name,i));

    shift += GetSize(desc_p, NULL);
  }
  void* ptr = new char[shift];
  mC.SetData((char*)ptr,shift);

  ZeroVarField();
}
//--------------------------------------------------------------------
void* TMarkUpContainer::Get(const char* name, bool type_counter, int index)
{
  char* ptr = (char*)GetPtr();

  TMapStrInt::iterator fit = mMarkUp.find(name);
  if(fit==mMarkUp.end())
    return NULL;
  
  TDesc_Private& desc = mVectorSection.AT(fit->second);
  ptr += desc.shift;
  
  if((desc.c.type ==eVar)&&
     (type_counter==false))
  {
    // ��������
    int cnt = GetCountBy(ptr,desc.c.v.sizeCnt);
    if(index>=cnt) return NULL;// �������

    ptr += desc.c.v.sizeCnt;
    ptr += index*desc.c.v.sizeVar;
  }
  
  return ptr;
}
//--------------------------------------------------------------------
void* TMarkUpContainer::GetPtr()
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
    TDesc_Private& desc_p = mVectorSection.AT(i);// ����� �������� ������
    
    int sizeSection = GetSize(desc_p, ptr);// ���������� ������ ������

    if(desc_p.c.type==eVar)
    {
      desc_p.shift  = shift;
      desc_p.cntVar = GetCountBy(ptr,desc_p.c.v.sizeCnt);
    }

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
  mC.Done();

  mMarkUp.clear();
  mVectorSection.clear();
}
//--------------------------------------------------------------------
void TMarkUpContainer::Update()
{
  vector<int> vecDisVar;// ������ ��������������
  // ������� ���-�� �������������� �� ���� eVar
  int cntSection = mVectorSection.size();
  for(int i = 0 ; i < cntSection ; i++ )
  {
    TDesc_Private& desc_p = mVectorSection.AT(i);
    
    if(desc_p.c.type==eVar)
    {
      char* ptr = (char*)GetPtr()+desc_p.shift;
      int FreshCnt = GetCountBy(ptr,desc_p.c.v.sizeCnt);
      if(desc_p.cntVar!=FreshCnt)
        vecDisVar.push_back(i);
    }
  }
  //--------------------------------------------------------
  // ��������� ������ ��������������
  // ������� ���-�� �������������� �� ���� eVar
  int cntDisVar = vecDisVar.size();
  for(int d = 0 ; d < cntDisVar ; d++ )
  {
    int i = vecDisVar.AT(d);

    TDesc_Private& desc_p = mVectorSection.AT(i);
   
    char* ptr = (char*)GetPtr() + desc_p.shift; // ��������� �� ������ ������ (�������� � ������ ���-�� ��������� ������, ���-�� �������  ����������)
    int freshCnt = GetCountBy(ptr,desc_p.c.v.sizeCnt); // ��������� ���-�� �� ������

    int d_cntVar  = desc_p.cntVar - freshCnt;     // �������� �� ���-��
    int d_sizeVar = desc_p.c.v.sizeVar * d_cntVar;// �������� �� �������
    int new_size  = GetSize() - d_sizeVar;        // ����� ������

    void* newPtr = GetPtr();
    if(desc_p.cntVar < freshCnt)
    {
      int sizeBound = desc_p.shift + desc_p.c.v.sizeCnt + desc_p.c.v.sizeVar * desc_p.cntVar;
      // �������� ������ ������
      newPtr = mo_realloc_bound_new((char*)GetPtr(),GetSize(),sizeBound,-d_sizeVar);
    }
    else//if(desc_p.cntVar>freshCnt)
    {
      int sizeBound = desc_p.shift + desc_p.c.v.sizeCnt + desc_p.c.v.sizeVar * freshCnt;
      // ���������� �������
      char* ptrBoundLow = (char*)GetPtr() + sizeBound;
      int sizeMove      = GetSize() - sizeBound - d_sizeVar;
      memmove(ptrBoundLow, ptrBoundLow+d_sizeVar, sizeMove);
    }
    mC.Zero();
    mC.SetData((char*)newPtr,new_size);
    delete[](char*)newPtr;
    desc_p.cntVar=freshCnt;
    // �������� ������ ��� ������� �����
    for(int j = i+1 ; j < cntSection ; j++ )
    {
      mVectorSection.AT(j).shift -= d_sizeVar;    
    }
  }
}
//--------------------------------------------------------------------
int TMarkUpContainer::GetCountBy(char* ptr, int sizeCnt)
{
  int res = 0;
  switch(sizeCnt)
  {
    case 1:
      res = *ptr;
      break;
    case 2:
      res = *((short*)ptr);
      break;
    case 4:
      res = *((int*)ptr);
      break;
      default:/*�������*/;
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
    default:/*�������*/;
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
        res += desc_p.c.v.sizeVar*GetCountBy(ptr,desc_p.c.v.sizeCnt);
      break;
  }

  return res;
}
//--------------------------------------------------------------------
void TMarkUpContainer::ZeroVarField()
{
  char* ptr = (char*)GetPtr(); 
  int cnt = mVectorSection.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    TDesc_Private& desc_p = mVectorSection.AT(i);
    if(desc_p.c.type==eVar)
    {
      SetCountBy(ptr+desc_p.shift,desc_p.c.v.sizeCnt,0);
    }
  }
}
//--------------------------------------------------------------------