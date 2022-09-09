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

#include "StateManager9.h"
#include "Define_DX.h"


TStateManager9::TStateManager9()
{
  m_pStateBlock = NULL;
}
//--------------------------------------------------------------------------
TStateManager9::~TStateManager9()
{

}
//--------------------------------------------------------------------------
HRESULT TStateManager9::Reset(IDirect3DDevice9* pd3d9Device)
{
  HRESULT hr;
  V_RETURN( pd3d9Device->CreateStateBlock( D3DSBT_ALL, &m_pStateBlock ) );
  return S_OK;
}
//--------------------------------------------------------------------------
void TStateManager9::Lost()
{
  SAFE_RELEASE( m_pStateBlock );
}
//--------------------------------------------------------------------------
HRESULT TStateManager9::Push()
{
  HRESULT hr;
  V_RETURN( m_pStateBlock->Capture() );
  return S_OK;
}
//--------------------------------------------------------------------------
HRESULT TStateManager9::Pop()
{
  HRESULT hr;
  V_RETURN( m_pStateBlock->Apply() );
  return S_OK;
}
//--------------------------------------------------------------------------
