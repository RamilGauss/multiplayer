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
class TEvent_Packet : public TBasePacket
{
	// ushort type
public:
	TEvent_Packet(unsigned short type)
	{
		mType=type;
		mSize=sizeof(mType);
		mData=(char*)malloc(mSize);
		setType();
	};
};
//-----------------------------------------------------------------------------
//APPL_TYPE_Ñ_CHOOSE_TANK
class TÑ_Choose_Tank : public TBasePacket
{
  // ushort type, unsigned short currentTank
public:
  TÑ_Choose_Tank()
  {
    mType = APPL_TYPE_Ñ_CHOOSE_TANK;
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
////APPL_TYPE_C_READY_FOR_ROOM
//class TC_Ready_For_Room : public TBasePacket
//{
//  // ushort type
//public:
//  TC_Ready_For_Room()
//  {
//    mType=APPL_TYPE_C_READY_FOR_ROOM;
//    mSize=sizeof(mType);
//    mData=(char*)malloc(mSize);
//    setType();
//  };
//};
//-----------------------------------------------------------------------------

#endif