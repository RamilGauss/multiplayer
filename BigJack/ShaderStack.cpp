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

#include "ShaderStack.h"
#include "BL_Debug.h"

using namespace std;

TShaderStack::TShaderStack()
{

}
//-----------------------------------------------------------------------------------------------------
TShaderStack::~TShaderStack()
{
  Done();
}
//-----------------------------------------------------------------------------------------------------
int TShaderStack::Push(const char* nameValueIn, void* pDataIn, int sizeIn)
{
  TData* data = new TData;
  data->name = nameValueIn;
  data->Set((char*)pDataIn, sizeIn);
  mVecorNameData.push_back(data);
  return mVecorNameData.size()-1;
}
//-----------------------------------------------------------------------------------------------------
string TShaderStack::GetName(int index)
{
  BL_ASSERT(index<int(mVecorNameData.size()));
  return mVecorNameData.at(index)->name;
}
//-----------------------------------------------------------------------------------------------------
void* TShaderStack::GetData(int index, int& size)
{
  BL_ASSERT(index<int(mVecorNameData.size()));
  size = mVecorNameData.at(index)->size;
  return mVecorNameData.at(index)->data;
}
//-----------------------------------------------------------------------------------------------------
void TShaderStack::Done()
{
  int cnt = mVecorNameData.size();
  for(int i = 0 ; i < cnt ; i++)
    delete mVecorNameData.at(i);

  mVecorNameData.clear();
}
//-----------------------------------------------------------------------------------------------------
void TShaderStack::SetData(int index, void* pDataIn, int sizeIn)
{
  mVecorNameData.at(index)->Set((char*)pDataIn, sizeIn);
}
//-----------------------------------------------------------------------------------------------------
int TShaderStack::GetIndexByName(const char* name)
{
  int cnt = mVecorNameData.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    if(mVecorNameData.at(i)->name.compare(name)==0)
      return i;
  }
  BL_FIX_BUG();
  return -1;
}
//-----------------------------------------------------------------------------------------------------
