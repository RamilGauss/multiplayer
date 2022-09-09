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

#ifndef EditBoxWinH
#define EditBoxWinH

#include "DXUTgui.h"
#include <string>
#include "IEditBox.h"
#include "BaseGUI_Win.h"
#include "IGUI_Win.h"

class TEditBoxWin : public IEditBox, public IGUI_Win
{
private:
  CDXUTEditBox* mPrivateEditBox;
public:

  TEditBoxWin();
  virtual ~TEditBoxWin();

  // self
  virtual void SetText( const char* wszText, bool bSelected = false );
  virtual std::string GetText();
  virtual int GetTextLength();
  virtual void ClearText();
  virtual void SetTextColor( unsigned int cARGB );
  virtual void SetSelectedTextColor( unsigned int cARGB );
  virtual void SetSelectedBackColor( unsigned int cARGB );
  virtual void SetCaretColor( unsigned int cARGB );
  virtual void SetBorderWidth( int nBorder );
  virtual void SetSpacing( int nSpacing );
  virtual void ParseFloatArray( float* pNumbers, int nCount );
  virtual void SetTextFloatArray( const float* pNumbers, int nCount );

  // inherit
  BASE_GUI_WIN_DECL(mPrivateEditBox)

  
};


#endif
