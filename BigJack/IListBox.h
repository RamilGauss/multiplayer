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

#ifndef IListBoxH
#define IListBoxH

#include "IBaseGUI.h"
#include "Rect.h"

struct TListBoxItem
{
  char strText[256];
  void* pData;

  TRect rcActive;
  bool bSelected;
};

class IListBox : public IBaseGUI
{
public:

  IListBox(){};
  virtual ~IListBox(){};

  virtual unsigned int GetStyle() const = 0;
  virtual int GetSize() const = 0;
  virtual void SetStyle( unsigned int dwStyle ) = 0;
  virtual int GetScrollBarWidth() const = 0;
  virtual void SetScrollBarWidth( int nWidth ) = 0;
  virtual void SetBorder( int nBorder, int nMargin ) = 0;
  virtual bool AddItem( const char* wszText, void* pData ) = 0;
  virtual bool InsertItem( int nIndex, const char* wszText, void* pData ) = 0;
  virtual void RemoveItem( int nIndex ) = 0;
  virtual void RemoveItemByData( void* pData ) = 0;
  virtual void RemoveAllItems() = 0;
  
  virtual bool GetItem( TListBoxItem& item, int nIndex ) = 0;
  virtual int GetSelectedIndex( int nPreviousSelected = -1 ) = 0;
  virtual bool GetSelectedItem( TListBoxItem& item, int nPreviousSelected = -1 ) = 0;
  virtual void SelectItem( int nNewIndex ) = 0;

  enum STYLE
  {
    MULTISELECTION = 1
  };
  
};


#endif
