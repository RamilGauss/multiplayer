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

#include "DispTextDX.h"
#include "SDKmisc.h"

using namespace std;

TDispTextDX::TDispTextDX()
{
  mpd3dDevice = NULL;
  pFont = NULL;
  pTextSprite = NULL;
  bShow = true;

  mX = 5;
  mY = 5;
}
//-----------------------------------------------------
TDispTextDX::~TDispTextDX()
{

}
//-----------------------------------------------------
HRESULT TDispTextDX::CreateDevice(IDirect3DDevice9* pd3dDevice)
{
  HRESULT hr;
  mpd3dDevice = pd3dDevice;

  V_RETURN(SetupFont());

  return S_OK;
}
//-----------------------------------------------------
void TDispTextDX::Show()
{
  bShow = true;
}
//-----------------------------------------------------
void TDispTextDX::Hide()
{
  bShow = false;
}
//-----------------------------------------------------
void TDispTextDX::SetPos(int x, int y)
{
  mX = x;
  mY = y;
}
//-----------------------------------------------------
void TDispTextDX::SetText(wstring& str)
{
  mText = str;
}
//-----------------------------------------------------
void TDispTextDX::Render()
{
  if(bShow==false) return;

  CDXUTTextHelper txtHelper( pFont, pTextSprite, 15 );

  txtHelper.Begin();
  txtHelper.SetInsertionPos( mX, mY );
  txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
  txtHelper.DrawTextLine( mText.data() );

  txtHelper.End();
}
//-----------------------------------------------------
HRESULT TDispTextDX::SetupFont()
{
  HRESULT hr;
  V_RETURN( D3DXCreateFont( mpd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
    L"Arial", &pFont ) );

  return S_OK;
}
//-----------------------------------------------------
HRESULT TDispTextDX::Lost()
{
  HRESULT hr;
  if( pFont )
    V_RETURN( pFont->OnLostDevice());
  SAFE_RELEASE( pTextSprite );
  return S_OK;
}
//-----------------------------------------------------
HRESULT TDispTextDX::Destroy()
{
  SAFE_RELEASE( pFont );
  return S_OK;
}
//-----------------------------------------------------
HRESULT TDispTextDX::Reset()
{
  HRESULT hr;
  if( pFont )
    V_RETURN( pFont->OnResetDevice() );
  V_RETURN( D3DXCreateSprite( mpd3dDevice, &pTextSprite ) );
  return S_OK;
}
//-----------------------------------------------------
