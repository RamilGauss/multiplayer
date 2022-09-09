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

#include <atlconv.h>
#include "DXUT.h"
#include "ComboBoxWin.h"


TComboBoxWin::TComboBoxWin()
{
  mPrivateComboBox = new CDXUTComboBox;
}
//--------------------------------------------------------------------
TComboBoxWin::~TComboBoxWin()
{

}
//--------------------------------------------------------------------
bool TComboBoxWin::AddItem( const char* strText, void* pData )
{
  USES_CONVERSION;
  return (mPrivateComboBox->AddItem(A2W(strText),pData)==S_OK);
}
//--------------------------------------------------------------------
void TComboBoxWin::RemoveAllItems()
{
  mPrivateComboBox->RemoveAllItems();
}
//--------------------------------------------------------------------
void TComboBoxWin::RemoveItem( int index )
{
  mPrivateComboBox->RemoveItem((UINT)index);
}
//--------------------------------------------------------------------
bool TComboBoxWin::ContainsItem( const char* strText, int iStart )
{
  USES_CONVERSION;
  return mPrivateComboBox->ContainsItem(A2W(strText),(UINT)iStart);
}
//--------------------------------------------------------------------
int TComboBoxWin::FindItem( const char* strText, int iStart )
{
  USES_CONVERSION;
  return mPrivateComboBox->FindItem(A2W(strText),(UINT)iStart);
}
//--------------------------------------------------------------------
void* TComboBoxWin::GetItemData( const char* strText )
{
  USES_CONVERSION;
  return mPrivateComboBox->GetItemData(A2W(strText));
}
//--------------------------------------------------------------------
void* TComboBoxWin::GetItemData( int nIndex )
{
  return mPrivateComboBox->GetItemData((UINT)nIndex);
}
//--------------------------------------------------------------------
void TComboBoxWin::SetDropHeight( int nHeight )
{
  mPrivateComboBox->SetDropHeight(nHeight);
}
//--------------------------------------------------------------------
int TComboBoxWin::GetScrollBarWidth() const
{
  return mPrivateComboBox->GetScrollBarWidth();
}
//--------------------------------------------------------------------
void TComboBoxWin::SetScrollBarWidth( int nWidth )
{
  mPrivateComboBox->SetScrollBarWidth(nWidth);
}
//--------------------------------------------------------------------
int TComboBoxWin::GetSelectedIndex() const
{
  return mPrivateComboBox->GetSelectedIndex();
}
//--------------------------------------------------------------------
void* TComboBoxWin::GetSelectedData()
{
  return mPrivateComboBox->GetSelectedData();
}
//--------------------------------------------------------------------
bool TComboBoxWin::GetSelectedItem(TComboBoxItem& item)
{
  DXUTComboBoxItem* pItemWin = mPrivateComboBox->GetSelectedItem();
  if(pItemWin==NULL)
    return false;
  CopyItem(item,pItemWin);
  return true;
}
//--------------------------------------------------------------------
int TComboBoxWin::GetNumItems()
{
  return mPrivateComboBox->GetNumItems();
}
//--------------------------------------------------------------------
bool TComboBoxWin::GetItem( TComboBoxItem& item, int index )
{
  DXUTComboBoxItem* pItemWin = mPrivateComboBox->GetItem(index);
  if(pItemWin==NULL)
    return false;
  CopyItem(item,pItemWin);
  return true;
}
//--------------------------------------------------------------------
bool TComboBoxWin::SetSelectedByIndex( int index )
{
  return (mPrivateComboBox->SetSelectedByIndex(index)==S_OK);
}
//--------------------------------------------------------------------
bool TComboBoxWin::SetSelectedByText( const char* strText )
{
  USES_CONVERSION;
  return (mPrivateComboBox->SetSelectedByText(A2W(strText))==S_OK);
}
//--------------------------------------------------------------------
bool TComboBoxWin::SetSelectedByData( void* pData )
{
  return (mPrivateComboBox->SetSelectedByData(pData)==S_OK);
}
//--------------------------------------------------------------------
void TComboBoxWin::CopyItem(TComboBoxItem &item, DXUTComboBoxItem*pItemWin)
{
  item.bVisible = pItemWin->bVisible;
  item.pData    = pItemWin->pData;

  item.rcActive.bottom = pItemWin->rcActive.bottom;
  item.rcActive.left   = pItemWin->rcActive.left;
  item.rcActive.right  = pItemWin->rcActive.right;
  item.rcActive.top    = pItemWin->rcActive.top;
  USES_CONVERSION;
  strcpy(item.strText, W2A(pItemWin->strText));
}
//--------------------------------------------------------------------
BUTTON_GUI_WIN_DEF(TComboBoxWin,mPrivateComboBox)
//-----------------------------------------------------------------------
STATIC_GUI_WIN_DEF(TComboBoxWin,mPrivateComboBox)
//-----------------------------------------------------------------------
BASE_GUI_WIN_DEF(TComboBoxWin,nsTypeGUI::tComboBox,mPrivateComboBox)
//-----------------------------------------------------------------------