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

#include "ManagerResourceDX.h"
#include <atlconv.h>
#include "Logger.h"
#include <algorithm>

using namespace std;

TManagerResourceDX::TManagerResourceDX()
{

}
//--------------------------------------------------------------------
TManagerResourceDX::~TManagerResourceDX()
{

}
//--------------------------------------------------------------------
bool TManagerResourceDX::Set(int type, void* bufferIn)
{
  bool res = false;
  switch(type)
  {
    case eTypeShader:
      res = mManagerEffect.Set((char*)bufferIn);
      break;
    case eTypeTexture:
      break;
    default:;
  }
  return res;
}
//--------------------------------------------------------------------
void* TManagerResourceDX::Load(const wchar_t* sPath)
{
  void* pResult = NULL;
  int code_ext = DefType(sPath);
  switch(code_ext)
  {
    case eTypeShader:
      pResult = mManagerEffect.Load(sPath);
      break;
    case eTypeTexture:
      pResult = mManagerTexture.Load(sPath);
      break;
    default:;
  }
  return pResult;
}
//--------------------------------------------------------------------
void TManagerResourceDX::Reset()
{
  mManagerEffect.Reset();
  mManagerTexture.Reset();
}
//--------------------------------------------------------------------
void TManagerResourceDX::Lost()
{
  mManagerEffect.Lost();
  mManagerTexture.Lost();
}
//--------------------------------------------------------------------
void TManagerResourceDX::Destroy()
{
  mManagerEffect.Destroy();
  mManagerTexture.Destroy();
}
//--------------------------------------------------------------------
TManagerResourceDX::TypeResource TManagerResourceDX::DefType(const wchar_t* sPath)
{
  wstring ws = sPath;
  int off = ws.rfind(L".");
  int size = ws.size();
  if(size>off+1)
  {
    const wchar_t* pExtension = ws.data()+off+1;
    if((wcscmp(pExtension,L"fx") ==0)||
       (wcscmp(pExtension,L"fxo")==0))
      return eTypeShader;
    if((wcscmp(pExtension,L"dds") ==0)||
       (wcscmp(pExtension,L"jpg") ==0)||
       (wcscmp(pExtension,L"bmp") ==0)||
       (wcscmp(pExtension,L"png") ==0)||
       (wcscmp(pExtension,L"jpeg")==0))
      return eTypeTexture;
  }

  USES_CONVERSION;
  
  GetLogger()->Get("GE")->WriteF_time("��������� ��������. �������������� ����������. %s\n",W2A(sPath));
  return eUndef;
}
//--------------------------------------------------------------------
void TManagerResourceDX::Setup(IDirect3DDevice9* pd3dDevice)
{
  mManagerEffect.SetDevice(pd3dDevice);
  mManagerTexture.SetDevice(pd3dDevice);
}
//--------------------------------------------------------------------
