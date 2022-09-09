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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include "ManagerEffect.h"
#include "HiTimer.h"
#include "Logger.h"

using namespace std;


//#define LOG_TIME_COMPILE_SHADER

TManagerEffect::TManagerEffect()
{

}
//----------------------------------------------------------------------------
TManagerEffect::~TManagerEffect()
{

}
//----------------------------------------------------------------------------
void* TManagerEffect::Load(const wchar_t* sPath)
{
  void* p = NULL;
  HRESULT hr;
  map<wstring,ID3DXEffect*>::iterator fit = mMapPathResource.find(wstring(sPath));
  map<wstring,ID3DXEffect*>::iterator eit = mMapPathResource.end();
  if(fit==eit)// не нашли
  {
    // компилировать Shader
    ID3DXEffect* pEffect;

    // замер времени загрузки и компиляции шейдера
#ifdef LOG_TIME_COMPILE_SHADER
    guint32 start = ht_GetMSCount();
#endif

    DWORD dwShaderFlags = D3DX_PARAMETER_SHARED;
    V( D3DXCreateEffectFromFile( m_pd3dDevice, sPath, 
      NULL, NULL, dwShaderFlags, NULL, &pEffect, NULL ) );
#ifdef LOG_TIME_COMPILE_SHADER
    start = ht_GetMSCount() - start;
    GlobalLoggerGE.WriteF_time("ManagerEffect: LoadShader, compile=%u\n",start);
#endif

    p = pEffect;

    map<wstring,ID3DXEffect*>::value_type val(wstring(sPath),pEffect);
    mMapPathResource.insert(val);
  }
  else// дубликат
  {
    p = fit->second;
  }

  
  return p;
}
//----------------------------------------------------------------------------
void TManagerEffect::Reset()
{
  HRESULT hr;
  map<wstring,ID3DXEffect*>::iterator bit = mMapPathResource.begin();
  map<wstring,ID3DXEffect*>::iterator eit = mMapPathResource.end();
  while(bit!=eit)
  {
    ID3DXEffect* p = bit->second;
    V(p->OnResetDevice());
    bit++;
  }
}
//----------------------------------------------------------------------------
void TManagerEffect::Lost()
{
  HRESULT hr;
  map<wstring,ID3DXEffect*>::iterator bit = mMapPathResource.begin();
  map<wstring,ID3DXEffect*>::iterator eit = mMapPathResource.end();
  while(bit!=eit)
  {
    ID3DXEffect* p = bit->second;
    V(p->OnLostDevice());
    bit++;
  }
}
//----------------------------------------------------------------------------
void TManagerEffect::Destroy()
{
  map<wstring,ID3DXEffect*>::iterator bit = mMapPathResource.begin();
  map<wstring,ID3DXEffect*>::iterator eit = mMapPathResource.end();
  while(bit!=eit)
  {
    ID3DXEffect* pEffect = bit->second;
    SAFE_RELEASE(pEffect);
    bit++;
  }
  mMapPathResource.clear();
}
//----------------------------------------------------------------------------
bool TManagerEffect::Set(char* bufferIn)
{
  TShaderStack* pSS = (TShaderStack*)bufferIn;
  map<wstring, ID3DXEffect*>::iterator bit = mMapPathResource.begin();
  map<wstring, ID3DXEffect*>::iterator eit = mMapPathResource.end();
  while(bit!=eit)
  {
    ID3DXEffect* pEffect = bit->second;
    mExecShaderStack.Set(pEffect,pSS);
    bit++;
  }
  return true;
}
//----------------------------------------------------------------------------