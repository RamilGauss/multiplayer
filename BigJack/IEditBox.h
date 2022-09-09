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

#ifndef IEditBoxH
#define IEditBoxH

#include "IBaseGUI.h"

class IEditBox : public IBaseGUI
{
public:

  IEditBox(){};
  virtual ~IEditBox(){};

  virtual void SetText( const char* wszText, bool bSelected = false ) = 0;
  virtual std::string GetText() = 0;
  virtual int GetTextLength() = 0;
  virtual void ClearText() = 0;
  virtual void SetTextColor( unsigned int cARGB ) = 0;
  virtual void SetSelectedTextColor( unsigned int cARGB ) = 0;
  virtual void SetSelectedBackColor( unsigned int cARGB ) = 0;
  virtual void SetCaretColor( unsigned int cARGB ) = 0;
  virtual void SetBorderWidth( int nBorder ) = 0;
  virtual void SetSpacing( int nSpacing ) = 0;
  virtual void ParseFloatArray( float* pNumbers, int nCount ) = 0;
  virtual void SetTextFloatArray( const float* pNumbers, int nCount ) = 0;
  
};


#endif
