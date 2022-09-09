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

#include "DXUT.h"
#include "EditBoxWin.h"
#include <atlconv.h>

TEditBoxWin::TEditBoxWin()
{
  mPrivateEditBox = new CDXUTEditBox;
}
//-----------------------------------------------------------------------
TEditBoxWin::~TEditBoxWin()
{

}
//-----------------------------------------------------------------------
void TEditBoxWin::SetText( const char* wszText, bool bSelected )
{
  USES_CONVERSION;
  mPrivateEditBox->SetText(A2W(wszText), bSelected);
}
//-----------------------------------------------------------------------
std::string TEditBoxWin::GetText()
{
  std::string str;
  USES_CONVERSION;
  str = W2A(mPrivateEditBox->GetText());
  return str;
}
//-----------------------------------------------------------------------
int TEditBoxWin::GetTextLength()
{
  return mPrivateEditBox->GetTextLength();
}
//-----------------------------------------------------------------------
void TEditBoxWin::ClearText()
{
  mPrivateEditBox->ClearText();
}
//-----------------------------------------------------------------------
void TEditBoxWin::SetTextColor( unsigned int cARGB )
{
  mPrivateEditBox->SetTextColor(cARGB);
}
//-----------------------------------------------------------------------
void TEditBoxWin::SetSelectedTextColor( unsigned int cARGB )
{
  mPrivateEditBox->SetSelectedTextColor( cARGB );
}
//-----------------------------------------------------------------------
void TEditBoxWin::SetSelectedBackColor( unsigned int cARGB )
{
  mPrivateEditBox->SetSelectedBackColor( cARGB );
}
//-----------------------------------------------------------------------
void TEditBoxWin::SetCaretColor( unsigned int cARGB )
{
  mPrivateEditBox->SetCaretColor( cARGB );
}
//-----------------------------------------------------------------------
void TEditBoxWin::SetBorderWidth( int nBorder )
{
  mPrivateEditBox->SetBorderWidth( nBorder );
}
//-----------------------------------------------------------------------
void TEditBoxWin::SetSpacing( int nSpacing )
{
  mPrivateEditBox->SetSpacing( nSpacing );
}
//-----------------------------------------------------------------------
void TEditBoxWin::ParseFloatArray( float* pNumbers, int nCount )
{
  mPrivateEditBox->ParseFloatArray(pNumbers,nCount);
}
//-----------------------------------------------------------------------
void TEditBoxWin::SetTextFloatArray( const float* pNumbers, int nCount )
{
  mPrivateEditBox->SetTextFloatArray(pNumbers,nCount);
}
//-----------------------------------------------------------------------
BASE_GUI_WIN_DEF(TEditBoxWin,nsTypeGUI::tEditBox,mPrivateEditBox)
//-----------------------------------------------------------------------
