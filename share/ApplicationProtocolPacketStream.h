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
// APPL_TYPE_S_COUNT_DOWN
class TS_Count_Down : public TBasePacket
{
	// ushort type, float second_rest

public:
	TS_Count_Down(){mType=APPL_TYPE_S_COUNT_DOWN;
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
  // вектор относительно башни указателя мыши
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


#endif