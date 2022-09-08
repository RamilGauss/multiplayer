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