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
#include "ListBoxWin.h"
#include <atlconv.h>


TListBoxWin::TListBoxWin()
{
  mPrivateListBox = new CDXUTListBox;
}
//-------------------------------------------------------------------------
TListBoxWin::~TListBoxWin()
{

}
//-------------------------------------------------------------------------
unsigned int TListBoxWin::GetStyle() const
{
  return mPrivateListBox->GetStyle();
}
//-------------------------------------------------------------------------
int TListBoxWin::GetSize() const
{
  return mPrivateListBox->GetSize();
}
//-------------------------------------------------------------------------
void TListBoxWin::SetStyle( unsigned int dwStyle )
{
  mPrivateListBox->SetStyle(dwStyle);
}
//-------------------------------------------------------------------------
int TListBoxWin::GetScrollBarWidth() const
{
  return mPrivateListBox->GetScrollBarWidth();
}
//-------------------------------------------------------------------------
void TListBoxWin::SetScrollBarWidth( int nWidth )
{
  mPrivateListBox->SetScrollBarWidth(nWidth);
}
//-------------------------------------------------------------------------
void TListBoxWin::SetBorder( int nBorder, int nMargin )
{
  mPrivateListBox->SetBorder(nBorder, nMargin);
}
//-------------------------------------------------------------------------
bool TListBoxWin::AddItem( const char* wszText, void* pData )
{
  USES_CONVERSION;
  return (mPrivateListBox->AddItem(A2W(wszText),pData)==S_OK);
}
//-------------------------------------------------------------------------
bool TListBoxWin::InsertItem( int nIndex, const char* wszText, void* pData )
{
  USES_CONVERSION;
  return (mPrivateListBox->InsertItem(nIndex,A2W(wszText),pData)==S_OK);
}
//-------------------------------------------------------------------------
void TListBoxWin::RemoveItem( int nIndex )
{
  mPrivateListBox->RemoveItem(nIndex);
}
//-------------------------------------------------------------------------
void TListBoxWin::RemoveItemByData( void* pData )
{
  mPrivateListBox->RemoveItemByData(pData);
}
//-------------------------------------------------------------------------
void TListBoxWin::RemoveAllItems()
{
  mPrivateListBox->RemoveAllItems();
}
//-------------------------------------------------------------------------
bool TListBoxWin::GetItem( TListBoxItem& item, int nIndex )
{
  DXUTListBoxItem* pItemWin = mPrivateListBox->GetItem(nIndex);
  if(pItemWin==NULL)
    return false;
  CopyItem(item,pItemWin);
  return true;
}
//-------------------------------------------------------------------------
int TListBoxWin::GetSelectedIndex( int nPreviousSelected)
{
  return mPrivateListBox->GetSelectedIndex(nPreviousSelected);
}
//-------------------------------------------------------------------------
bool TListBoxWin::GetSelectedItem( TListBoxItem& item, int nPreviousSelected )
{
  DXUTListBoxItem* pItemWin = mPrivateListBox->GetSelectedItem(nPreviousSelected);
  if(pItemWin==NULL)
    return false;
  CopyItem(item,pItemWin);
  return true;
}
//-----------------------------------------------------------------------
void TListBoxWin::SelectItem( int nNewIndex )
{
  mPrivateListBox->SelectItem(nNewIndex);
}
//-----------------------------------------------------------------------
void TListBoxWin::CopyItem(TListBoxItem &item, DXUTListBoxItem*pItemWin)
{
  item.bSelected = pItemWin->bSelected;
  item.pData     = pItemWin->pData;

  item.rcActive.bottom = pItemWin->rcActive.bottom;
  item.rcActive.left   = pItemWin->rcActive.left;
  item.rcActive.right  = pItemWin->rcActive.right;
  item.rcActive.top    = pItemWin->rcActive.top;
  USES_CONVERSION;
  strcpy(item.strText, W2A(pItemWin->strText));
}
//-----------------------------------------------------------------------
BASE_GUI_WIN_DEF(TListBoxWin,nsTypeGUI::tListBox,mPrivateListBox)
//-----------------------------------------------------------------------