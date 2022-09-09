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

#ifndef ComboBoxWinH
#define ComboBoxWinH

#include "DXUTgui.h"
#include "IComboBox.h"
#include "BaseGUI_Win.h"
#include "IGUI_Win.h"


class TComboBoxWin : public IComboBox, public IGUI_Win
{
private:
  CDXUTComboBox* mPrivateComboBox;
public:

  TComboBoxWin();
  virtual ~TComboBoxWin();

  // self
  virtual bool AddItem( const char* strText, void* pData );
  virtual void RemoveAllItems();
  virtual void RemoveItem( int index );
  virtual bool ContainsItem( const char* strText, int iStart=0 );
  virtual int  FindItem( const char* strText, int iStart=0 );
  virtual void* GetItemData( const char* strText );
  virtual void* GetItemData( int nIndex );
  virtual void  SetDropHeight( int nHeight );
  virtual int   GetScrollBarWidth() const;
  virtual void  SetScrollBarWidth( int nWidth );
  virtual int   GetSelectedIndex() const;
  virtual void* GetSelectedData();
  virtual bool GetSelectedItem(TComboBoxItem& item);
  virtual int GetNumItems();
  virtual bool GetItem( TComboBoxItem& item, int index );

  virtual bool SetSelectedByIndex( int index );
  virtual bool SetSelectedByText( const char* strText );
  virtual bool SetSelectedByData( void* pData );
  //--------------------------------------------------
  // inherit
  BUTTON_GUI_WIN_DECL
  STATIC_GUI_WIN_DECL
  BASE_GUI_WIN_DECL(mPrivateComboBox)

protected:

  void CopyItem(TComboBoxItem &item, DXUTComboBoxItem*pItemWin);
};


#endif
