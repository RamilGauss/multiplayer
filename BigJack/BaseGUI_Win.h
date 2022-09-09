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

#ifndef IBaseGUI_WinH
#define IBaseGUI_WinH



#define BASE_GUI_WIN_DECL(mPrivateDXUT) \
virtual void* GetPrivate(){return mPrivateDXUT;} \
virtual void SetEnabled( bool bEnabled ); \
virtual bool GetEnabled(); \
virtual void SetVisible( bool bVisible ); \
virtual bool GetVisible(); \
virtual nsTypeGUI::tTypeGUI GetType() const; \
virtual void SetLocation( int x, int y ); \
virtual void SetSize( int width, int height ); \
virtual void SetUserData( void* pUserData ); \
virtual void* GetUserData() const; \
//-----------------------------------------------------------------------
#define BASE_GUI_WIN_DEF(classname,typeGUI,mPrivateDXUT) \
void classname::SetEnabled( bool bEnabled ) \
{ \
  mPrivateDXUT->SetEnabled(bEnabled); \
} \
bool classname::GetEnabled() \
{ \
  return mPrivateDXUT->GetEnabled(); \
} \
void classname::SetVisible( bool bVisible ) \
{ \
  mPrivateDXUT->SetVisible(bVisible); \
} \
bool classname::GetVisible() \
{ \
  return mPrivateDXUT->GetVisible(); \
} \
nsTypeGUI::tTypeGUI classname::GetType() const \
{ \
  return typeGUI; \
} \
void classname::SetLocation( int x, int y ) \
{ \
  mPrivateDXUT->SetLocation(x,y); \
} \
void classname::SetSize( int width, int height ) \
{ \
  mPrivateDXUT->SetSize(width,height); \
} \
void classname::SetUserData( void* pUserData ) \
{ \
  mPrivateDXUT->SetUserData(pUserData); \
} \
void* classname::GetUserData() const \
{ \
  return mPrivateDXUT->GetUserData(); \
}

//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#define STATIC_GUI_WIN_DECL \
virtual std::string GetText(); \
virtual bool SetText( const char* strText );
//-----------------------------------------------------------------------
#define STATIC_GUI_WIN_DEF(classname,mPrivateDXUT) \
std::string classname::GetText() \
{ \
  std::string str; \
  USES_CONVERSION; \
  str = W2A(mPrivateDXUT->GetText()); \
  return str; \
} \
bool classname::SetText( const char* strText ) \
{ \
  USES_CONVERSION; \
  return (mPrivateDXUT->SetText(A2W(strText))==S_OK); \
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#define BUTTON_GUI_WIN_DECL \
virtual bool IsPressed();
//-----------------------------------------------------------------------
#define BUTTON_GUI_WIN_DEF(classname,mPrivateDXUT) \
bool classname::IsPressed() \
{ \
  return mPrivateDXUT->IsPressed(); \
}
//-----------------------------------------------------------------------
//-----------------------------------------------------------------------
#define CHECKBOX_GUI_WIN_DECL \
virtual bool GetChecked(); \
virtual void SetChecked( bool bChecked );
//-----------------------------------------------------------------------
#define CHECKBOX_GUI_WIN_DEF(classname,mPrivateDXUT) \
bool classname::GetChecked() \
{ \
  return mPrivateDXUT->GetChecked(); \
} \
void classname::SetChecked( bool bChecked ) \
{ \
  mPrivateDXUT->SetChecked(bChecked); \
}




#endif
