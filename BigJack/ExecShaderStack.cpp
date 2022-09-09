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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "ExecShaderStack.h"
#include "ShaderStack.h"
#include "BL_Debug.h"

using namespace std;

TExecShaderStack::TExecShaderStack()
{

}
//-----------------------------------------------------------------------------------------------------
TExecShaderStack::~TExecShaderStack()
{

}
//-----------------------------------------------------------------------------------------------------
void TExecShaderStack::Set(ID3DXEffect* pEffect, TShaderStack* shaderStack)
{
  HRESULT hr;
  // ��������� ���� �� ����� ������
  TMAP_EFFECT::iterator fit = mMapEffect.find(pEffect);
  TMAP_EFFECT::iterator eit = mMapEffect.end();
  if(fit==eit)
  {
    // �� �����
    TMAP_HANDLE mS_D3H;
    TMAP_EFFECT::value_type val(pEffect,mS_D3H);
    mMapEffect.insert(val);
    fit = mMapEffect.find(pEffect);
  }

  TMAP_HANDLE* pMapStrHandle = &(fit->second);

  int cnt = shaderStack->Count();
  for(int i = 0 ; i < cnt ; i++)
  {
    string str = shaderStack->GetName(i);
    TMAP_HANDLE::iterator mip = pMapStrHandle->find(str);
    TMAP_HANDLE::iterator meip = pMapStrHandle->end();
    D3DXHANDLE handle;
    if(mip!=meip)
    {
      handle = mip->second;
    }
    else
    {
      handle = pEffect->GetParameterBySemantic(0,str.data());
      BL_ASSERT(handle);
      TMAP_HANDLE::value_type val_handle(str,handle);
      pMapStrHandle->insert(val_handle);
    }
    int sizeData;
    void* pData = shaderStack->GetData(i,sizeData);
    V(pEffect->SetValue(handle,pData,sizeData));
  }
}
//-----------------------------------------------------------------------------------------------------