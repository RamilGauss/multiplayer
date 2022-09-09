/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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

#include "ManagerTexture.h"

using namespace std;


TManagerTexture::TManagerTexture()
{

}
//--------------------------------------------------------------------------
TManagerTexture::~TManagerTexture()
{

}
//--------------------------------------------------------------------------
void* TManagerTexture::Load(const wchar_t* sPath)
{
  HRESULT hr;
  IDirect3DTexture9* pTexture = NULL;
  map<wstring,IDirect3DTexture9*>::iterator fit = mMapPathResource.find(wstring(sPath));
  map<wstring,IDirect3DTexture9*>::iterator eit = mMapPathResource.end();
  if(fit==eit)// не нашли
  {
    V( D3DXCreateTextureFromFile( m_pd3dDevice, sPath,
      &pTexture ) );

    map<wstring,IDirect3DTexture9*>::value_type val(wstring(sPath),pTexture);
    mMapPathResource.insert(val);
  }
  else// дубликат
  {
    pTexture = fit->second;
  }
  return pTexture;
}
//--------------------------------------------------------------------------
void TManagerTexture::Reset()
{
  
}
//----------------------------------------------------------------------------
void TManagerTexture::Lost()
{

}
//----------------------------------------------------------------------------
void TManagerTexture::Destroy()
{
  map<wstring,IDirect3DTexture9*>::iterator bit = mMapPathResource.begin();
  map<wstring,IDirect3DTexture9*>::iterator eit = mMapPathResource.end();
  while(bit!=eit)
  {
    IDirect3DTexture9* pTexture = bit->second;
    SAFE_RELEASE(pTexture);
    bit++;
  }
  mMapPathResource.clear();
}
//----------------------------------------------------------------------------
bool TManagerTexture::Set(char* bufferIn)
{
  return true;  
}
//----------------------------------------------------------------------------