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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef ApplicationProtocolPacketCmdH
#define ApplicationProtocolPacketCmdH

#include "ApplicationProtocolPacket.h"
#include "ApplicationProtocolDef.h"

// APPL_TYPE_R_FIRE
// APPL_TYPE_C_PRESS_LEFT     
// APPL_TYPE_C_RELEASE_LEFT   
// APPL_TYPE_C_PRESS_DOWN     
// APPL_TYPE_C_RELEASE_DOWN   
// APPL_TYPE_C_PRESS_RIGHT    
// APPL_TYPE_C_RELEASE_RIGHT  
// APPL_TYPE_C_PRESS_UP       
// APPL_TYPE_C_RELEASE_UP     
class TC_Key_Event : public TBasePacket
{
	// ushort type
  // uint event

public:
	TC_Key_Event()
	{
		mType=APPL_TYPE_C_KEY_EVENT;
		mSize=sizeof(mType);
		mData=(char*)malloc(mSize);
		setType();
	}

  void getKeyEvent(unsigned int val){ *(getPointerKeyEvent())= val;}
  unsigned int getKeyEvent(){return *(getPointerKeyEvent());}
  unsigned int* getPointerKeyEvent()
  {
    return (unsigned int*)(mData+sizeof(mType));
  }
};
//-----------------------------------------------------------------------------
//APPL_TYPE_С_CHOOSE_TANK
class TС_Choose_Tank : public TBasePacket
{
  // ushort type, unsigned short currentTank
public:
  TС_Choose_Tank()
  {
    mType = APPL_TYPE_С_CHOOSE_TANK;
    mSize = sizeof(mType)+sizeof(unsigned short);
    mData = (char*)malloc(mSize);
    setType();
  }
  //-----------------------------------------
  void setCurrentTank(unsigned short val)
  {
    *getPointerCurrentTank() = val;
  }
  //-----------------------------------------
  unsigned short getCurrentTank()
  {
    return *getPointerCurrentTank();
  }
  //-----------------------------------------
  unsigned short* getPointerCurrentTank()
  {
     return (unsigned short*)(mData+sizeof(mType));
  }

};
//-----------------------------------------------------------------------------

#endif