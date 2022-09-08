/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "LoaderListModel.h"
#include "BL_Debug.h"


TLoaderListModel::TLoaderListModel()
{
  mCurDirectory[0] = '\0';
  mSize = 0;
  pData = NULL;
}
//--------------------------------------------------------------------------------------
TLoaderListModel::~TLoaderListModel()
{
  ClearBuffer();
}
//--------------------------------------------------------------------------------------
bool TLoaderListModel::Open(char* sPath)
{
  GetCurrentDirectoryA(sizeof(mCurDirectory),mCurDirectory);
  int len = strlen(mCurDirectory);
  for(int i = len-1 ; i >= 0 ; i--)
  {
    if(mCurDirectory[i]=='\\')
    {
      mCurDirectory[i]='\0';
      break;
    }
  }

  bool res = lfHDD.ReOpen(sPath);
  if(!res) return false;

  return true;
}
//--------------------------------------------------------------------------------------
bool TLoaderListModel::Load(char*** pPointerArr,int& mCntPathModel)
{
  ReadData();

  mCntPath = 0;
  // ���� ���-�� �������
  if(!FindCnt()) return false;
  
  char** pArr = new char*[mCntPath];
  *pPointerArr = pArr;

  int size = 0;
  char* p = FindSeparator(pData,size);// ���������� �������� ���-��
  p = FindSeparator(p,size);
  int i = 0;
  while(p)
  {
    if(size)
    {
      int lenStr = size+strlen(mCurDirectory)+strlen("\\model\\");
      pArr[i] = new char[lenStr+1];
      memcpy(pArr[i],mCurDirectory,strlen(mCurDirectory));
      memcpy(pArr[i]+strlen(mCurDirectory),"\\model\\",strlen("\\model\\"));
      memcpy(pArr[i]+strlen(mCurDirectory)+strlen("\\model\\"),p,size);
      char* str = pArr[i];
      str[lenStr] = '\0';
      i++;
    }
    p = FindSeparator(p,size);
  }

  if(i!=mCntPath) return false;

  mCntPathModel = mCntPath;
  return true;
}
//--------------------------------------------------------------------------------------
bool TLoaderListModel::FindCnt()
{
  int size = 0;
  char* p = FindSeparator(pData,size);
  if(p==NULL) return false;
  char buffer[300];
  memcpy(buffer,p,size);
  buffer[size] = '\0';
  mCntPath = atoi(buffer);
  if(mCntPath==0) return false;
  return true;
}
//--------------------------------------------------------------------------------------
void TLoaderListModel::ReadData()
{
  ClearBuffer();

  mSize = lfHDD.Size();
  pData = new char[mSize];
  lfHDD.Read(pData,mSize);
}
//--------------------------------------------------------------------------------------
void TLoaderListModel::ClearBuffer()
{
  mSize = 0;
  delete []pData;
  pData = NULL;
}
//--------------------------------------------------------------------------------------
char* TLoaderListModel::FindSeparator(char* pBegin,int &size)
{
  // *������0*������1*...
  char* pFound = NULL;
  int i = (int)(pBegin-pData);
  BL_ASSERT(i>=0);
  for(; i < mSize ; i++)
  {
    if(pData[i]=='*')
    {
      i++;
      pFound = &pData[i];
      for( int j = i ;j < mSize ; j++)
      {
        if(pData[j]=='*')
        {
          size = j - i;
          return pFound;
        }
      }
      size = mSize - i;
      return pFound;
    }
  }
  return pFound;
}
//--------------------------------------------------------------------------------------