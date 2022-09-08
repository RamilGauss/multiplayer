/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef DescEventH
#define DescEventH

#include "TypeDef.h"
#include "Container.h"

namespace nsEvent
{

#ifdef WIN32
#pragma pack(push, 1)
#endif

struct SHARE_EI TEvent
{
  int from;
  TContainer container;
};

typedef enum
{
  eMouse    = 0,
  eKeyBoard = 1,
}tTypeEvent;
typedef enum
{
  eKeyEmpty = 0,
  eAlt   = 1<<1,
  eCtrl  = 1<<2,
  eShift = 1<<3,
}tKeyModifier;
typedef enum
{
  eMouseEmpty = 0,
  eLClick = 1<<1,
  eMClick = 1<<2,
  eRClick = 1<<3,
}tMouseButton;
typedef enum
{
  eButtonDown     = 1<<1,
  eButtonUp       = 1<<2,
  eButtonDblClick = 1<<3,
  eWheel          = 1<<4,
  eMove           = 1<<5,
}tTypeMouseEvent;
//---------------------------------------------------------------------------------
struct SHARE_EI TBaseEvent
{
  tTypeEvent type;
}_PACKED;
struct SHARE_EI TKeyEvent : public TBaseEvent
{
  int key;
  bool pressed/*or released*/;
  tKeyModifier modifier;
  TKeyEvent(){type=eKeyBoard;}
}_PACKED;
struct SHARE_EI TMouseEvent : public TBaseEvent
{
  tTypeMouseEvent state;
  int x;
  int y;
  tMouseButton button;
  int delta_wheel;
  TMouseEvent(){type=eMouse;}
}_PACKED;

#ifdef WIN32
#pragma pack(pop)
#endif

}
//------------------------------------------------------------------------------------------
namespace nsKey
{
  enum
  {
    e_1,
    e_2,
    e_3,
    e_4,
    e_5,
    e_6,
    e_7,
    e_8,
    e_9,
    e_0,
    e_A,
    e_B,
    e_C,
    e_D,
    e_E,
    e_G,
    e_F,
    e_I,
    e_H,
    e_J,
    e_K,
    e_L,
    e_M,
    e_N,
    e_O,
    e_P,
    e_Q,
    e_R,
    e_S,
    e_T,
    e_U,
    e_W,
    e_V,
    e_X,
    e_Y,
    e_Z,
    e_Space,
    e_Prior,
    e_Next,
    e_End,
    e_Home,
    e_Left,
    e_Up,
    e_Right,
    e_Down,
    e_Select,
    e_Print,
    e_Execute,
    e_Snapshot,
    e_Insert,
    e_Delete,
    e_Help,
    e_Back,
    e_Tab,
    e_Clear,
    e_Return,
    e_Shift,
    e_Control,
    e_Menu,
    e_Pause,
    e_Capital,
    e_Kana,
    e_Hangeul,
    e_Hangul,
    e_Junja,
    e_Final,
    e_Hanja,
    e_Kanji,
    e_Escape,
    e_Convert,
    e_Nonconvert,
    e_Accept,
    e_Modechange,
    e_LWin,
    e_RWin,
    e_Apps,
    e_Sleep,
    e_Numpad0,
    e_Numpad1,
    e_Numpad2,
    e_Numpad3,
    e_Numpad4,
    e_Numpad5,
    e_Numpad6,
    e_Numpad7,
    e_Numpad8,
    e_Numpad9,
    e_Multiply,
    e_Add,   
    e_Separator,
    e_Subtract,
    e_Decimal,
    e_Divide,
    e_F1,
    e_F2,
    e_F3,
    e_F4,
    e_F5,
    e_F6,
    e_F7,
    e_F8,
    e_F9,
    e_F10,
    e_F11,
    e_F12,
    e_F13,
    e_F14,
    e_F15,
    e_F16,
    e_F17,
    e_F18,
    e_F19,
    e_F20,
    e_F21,
    e_F22,
    e_F23,
    e_F24,
    e_Numlock,
    e_Scroll,
    e_LShift,
    e_RShift,
    e_LControl,
    e_RControl,
    e_LMenu,
    e_RMenu,
  };
}

#endif