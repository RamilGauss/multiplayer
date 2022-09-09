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

#include "ViewerFPS.h"
#include "SDKmisc.h"


TViewerFPS::TViewerFPS()
{
  mpd3dDevice = NULL;
  pFont = NULL;
  pTextSprite = NULL;
  bShow = true;
}
//-----------------------------------------------------
TViewerFPS::~TViewerFPS()
{

}
//-----------------------------------------------------
HRESULT TViewerFPS::CreateDevice(IDirect3DDevice9* pd3dDevice)
{
  HRESULT hr;
  mpd3dDevice = pd3dDevice;

  V_RETURN(SetupFont());

  return S_OK;
}
//-----------------------------------------------------
void TViewerFPS::Show()
{
  bShow = true;
}
//-----------------------------------------------------
void TViewerFPS::Hide()
{
  bShow = false;
}
//-----------------------------------------------------
void TViewerFPS::SetPos()
{

}
//-----------------------------------------------------
void TViewerFPS::Render(float FPS)
{
  if(bShow==false) return;

  wchar_t sFPS[100];
  swprintf_s(sFPS,L"FPS:%0.2f",FPS);
  CDXUTTextHelper txtHelper( pFont, pTextSprite, 15 );

  // Output statistics
  txtHelper.Begin();
  txtHelper.SetInsertionPos( 5, 5 );
  txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
  txtHelper.DrawTextLine( sFPS );

  txtHelper.End();
}
//-----------------------------------------------------
HRESULT TViewerFPS::SetupFont()
{
  HRESULT hr;
  V_RETURN( D3DXCreateFont( mpd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET,
    OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
    L"Arial", &pFont ) );

  return S_OK;
}
//-----------------------------------------------------
HRESULT TViewerFPS::Lost()
{
  HRESULT hr;
  if( pFont )
    V_RETURN( pFont->OnLostDevice());
  SAFE_RELEASE( pTextSprite );
  return S_OK;
}
//-----------------------------------------------------
HRESULT TViewerFPS::Destroy()
{
  SAFE_RELEASE( pFont );
  return S_OK;
}
//-----------------------------------------------------
HRESULT TViewerFPS::Reset()
{
  HRESULT hr;
  if( pFont )
    V_RETURN( pFont->OnResetDevice() );
  V_RETURN( D3DXCreateSprite( mpd3dDevice, &pTextSprite ) );
  return S_OK;
}
//-----------------------------------------------------
