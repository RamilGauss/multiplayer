#ifndef DescEventH
#define DescEventH

#include "..\GBaseLib\TypeDef.h"
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

#endif