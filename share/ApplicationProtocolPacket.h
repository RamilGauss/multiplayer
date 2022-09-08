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


#ifndef ApplicationProtocolPacketH
#define ApplicationProtocolPacketH

#include "ApplicationProtocolDef.h"
#include <stddef.h>
#include <string.h>
#include <malloc.h>
#include <memory.h>
#include "ServerStruct.h"
#include "GlobalParamsTank.h"

#if defined( WIN32 )
#pragma pack(push, 1)
#endif

// базовые структуры
//-----------------------------------------------------------------------------
//struct TDefTankInCommand_A_In_Fight
//{
//  char* sNick;
//  int lenNick;
//  unsigned char ID_Tank;
//  unsigned char typeGun;  // 0 - сток
//  unsigned char typeTower;// 0 - сток
//
//  TDefTankInCommand_A_In_Fight(){sNick = NULL;lenNick = 0;};
//  ~TDefTankInCommand_A_In_Fight(){free(sNick);sNick = NULL;lenNick = 0;};
//
//  int GetSize()
//  {
//    return sizeof(ID_Tank)+sizeof(typeGun)+sizeof(typeTower)+sizeof(lenNick)+lenNick;
//  }
//  char* GetData()
//  {
//
//  }
//};
//-----------------------------------------------------------------------------
class TBasePacket
{
protected:
  unsigned short mSize;
  char*          mData;

  unsigned short mType;

  void setType()
  {
    *((unsigned short*)(mData)) = mType;
  };
public:
	TBasePacket(){mData=NULL;mSize=0;};
  unsigned short getType(){return mType;};// см. ApplicationProtocolDef.h	
  
  virtual ~TBasePacket(){free(mData);};

  virtual int getSize(){return mSize;};
  virtual char* getData(){return mData;};
  virtual void setData(char* p, int size)
  {
    free(mData);
    mSize = size;
    mData = (char*)malloc(mSize);
    memcpy(mData,p,mSize);
  };
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------

#if defined( WIN32 )
#pragma pack(pop)
#endif

#endif