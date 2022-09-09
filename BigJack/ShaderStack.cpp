/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
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
  mVectorNameData.push_back(data);
  return mVectorNameData.size()-1;
}
//-----------------------------------------------------------------------------------------------------
string TShaderStack::GetName(int index)
{
  BL_ASSERT(index<int(mVectorNameData.size()));
  return mVectorNameData.at(index)->name;
}
//-----------------------------------------------------------------------------------------------------
void* TShaderStack::GetData(int index, int& size)
{
  BL_ASSERT(index<int(mVectorNameData.size()));
  size = mVectorNameData.at(index)->size;
  return mVectorNameData.at(index)->data;
}
//-----------------------------------------------------------------------------------------------------
void TShaderStack::Done()
{
  int cnt = mVectorNameData.size();
  for(int i = 0 ; i < cnt ; i++)
    delete mVectorNameData.at(i);

  mVectorNameData.clear();
}
//-----------------------------------------------------------------------------------------------------
void TShaderStack::SetData(int index, void* pDataIn, int sizeIn)
{
  mVectorNameData.at(index)->Set((char*)pDataIn, sizeIn);
}
//-----------------------------------------------------------------------------------------------------
int TShaderStack::GetIndexByName(const char* name)
{
  int cnt = mVectorNameData.size();
  for(int i = 0 ; i < cnt ; i++ )
  {
    if(mVectorNameData.at(i)->name.compare(name)==0)
      return i;
  }
  BL_FIX_BUG();
  return -1;
}
//-----------------------------------------------------------------------------------------------------
