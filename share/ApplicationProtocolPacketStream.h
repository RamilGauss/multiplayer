/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
2011, 2012
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
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef ApplicationProtocolPacketStreamH
#define ApplicationProtocolPacketStreamH

#include "ApplicationProtocolPacket.h"
#include "ApplicationProtocolDef.h"


//APPL_TYPE_S_GARAGE
class TS_Garage : public TBasePacket
{
	// ushort type, unsigned int fight, unsigned int common

public:
  TS_Garage(){mType=APPL_TYPE_S_GARAGE;              
              mSize = sizeof(mType)+sizeof(unsigned int)+sizeof(unsigned int);
              mData = (char*)malloc(mSize);
              setType();
  }

	unsigned int getFight(){return *getPointerFight();}; 
	unsigned int* getPointerFight(){return (unsigned int*)(mData+sizeof(mType));}; 
  void setFight(unsigned int val){*getPointerFight() = val;};

	unsigned int getCommon(){return *getPointerCommon();}; 
	unsigned int* getPointerCommon(){return (unsigned int*)(mData+sizeof(mType)+sizeof(unsigned int));}; 
  void setCommon(unsigned int val){*getPointerCommon() = val;};
};
//-----------------------------------------------------------------------------
//APPL_TYPE_S_FIGHT_COORD_BULLET см. ApplicationProtocolMainPacket.h
//-----------------------------------------------------------------------------
//APPL_TYPE_S_WAIT ,кол-во ожидающих в очереди, в бою и общее кол-во
class TS_Wait : public TBasePacket
{
  // ushort type, uint wait, uint fight, uint common

public:
  TS_Wait(){mType=APPL_TYPE_S_WAIT;
            mSize = sizeof(mType)+sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int);
            mData = (char*)malloc(mSize);
            setType();
  }

  unsigned int getWait(){return *getPointerWait();}; 
  unsigned int* getPointerWait(){return (unsigned int*)(mData+sizeof(mType));}; 
  void setWait(unsigned int val){*getPointerWait() = val;};

  unsigned int getFight(){return *getPointerFight();}; 
  unsigned int* getPointerFight(){return (unsigned int*)(mData+sizeof(mType)+sizeof(unsigned int));}; 
  void setFight(unsigned int val){*getPointerFight() = val;};

  unsigned int getCommon(){return *getPointerCommon();}; 
  unsigned int* getPointerCommon()
  {return (unsigned int*)(mData+sizeof(mType)+sizeof(unsigned int)+sizeof(unsigned int));}; 
  void setCommon(unsigned int val){*getPointerCommon() = val;};

};
//-----------------------------------------------------------------------------
// APPL_TYPE_G_S_COUNT_DOWN
class TS_Count_Down : public TBasePacket
{
	// ushort type, float second_rest

public:
	TS_Count_Down(){mType=APPL_TYPE_G_S_COUNT_DOWN;
	mSize = sizeof(mType)+sizeof(float);
	mData = (char*)malloc(mSize);
	setType();
	}

	float getRest(){return *getPointerRest();}; 
	float* getPointerRest(){return (float*)(mData+sizeof(mType));}; 
	void setWait(float val){*getPointerRest() = val;};
};
//-----------------------------------------------------------------------------
class TS_Orient_Aim : public TBasePacket
{
  // ushort type, 
  // вектор относительно башни указател€ мыши
  // float x
  // float y
  // float z

public:
  TS_Orient_Aim()
  {
    mType=APPL_TYPE_S_ORIENT_AIM;
    mSize = sizeof(mType)+sizeof(float)+sizeof(float)+sizeof(float);
    mData = (char*)malloc(mSize);
    setType();
  }

  void setXYZ(float x, float y, float z)
  {
    setX(x);
    setY(y);
    setZ(z);
  };

  void setX(float val){*getPointerX()=val;}
  float getX(){return *getPointerX();}
  float* getPointerX(){return (float*)(mData+sizeof(mType));}
  //---------------------------------------------------------
  void setY(float val){*getPointerY()=val;}
  float getY(){return *getPointerY();}
  float* getPointerY(){return (float*)(mData+sizeof(mType)+sizeof(float));}
  //---------------------------------------------------------
  void setZ(float val){*getPointerZ()=val;}
  float getZ(){return *getPointerZ();}
  float* getPointerZ(){return (float*)(mData+sizeof(mType)+sizeof(float)+sizeof(float));}
};
//-----------------------------------------------------------------------------
// APPL_TYPE_G_S_LOAD_MAP
class TS_Load_Map : public TBasePacket
{
  // ushort type,

public:
  TS_Load_Map(){mType=APPL_TYPE_G_S_LOAD_MAP;
  mSize = sizeof(mType);
  mData = (char*)malloc(mSize);
  setType();
  }
};
//-----------------------------------------------------------------------------


#endif