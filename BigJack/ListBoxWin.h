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

#ifndef ListBoxWinH
#define ListBoxWinH

#include "IListBox.h"
#include "DXUTgui.h"
#include "BaseGUI_Win.h"
#include "IGUI_Win.h"


class TListBoxWin : public IListBox, public IGUI_Win
{
private:

  CDXUTListBox* mPrivateListBox;

public:

  TListBoxWin();
  virtual ~TListBoxWin();
  
  // self
  virtual unsigned int GetStyle() const;
  virtual int GetSize() const;
  virtual void SetStyle( unsigned int dwStyle );
  virtual int GetScrollBarWidth() const;
  virtual void SetScrollBarWidth( int nWidth );
  virtual void SetBorder( int nBorder, int nMargin );
  virtual bool AddItem( const char* wszText, void* pData );
  virtual bool InsertItem( int nIndex, const char* wszText, void* pData );
  virtual void RemoveItem( int nIndex );
  virtual void RemoveItemByData( void* pData );
  virtual void RemoveAllItems();

  virtual bool GetItem( TListBoxItem& item, int nIndex );
  virtual int GetSelectedIndex( int nPreviousSelected = -1 );
  virtual bool GetSelectedItem( TListBoxItem& item, int nPreviousSelected = -1 );
  virtual void SelectItem( int nNewIndex );

  //--------------------------------------------------
  // inherit
  BASE_GUI_WIN_DECL(mPrivateListBox)
protected:
  void CopyItem(TListBoxItem &item, DXUTListBoxItem*pItemWin);
};


#endif
