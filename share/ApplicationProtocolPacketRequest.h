#ifndef ApplicationProtocolPacketRequestH
#define ApplicationProtocolPacketRequestH

#include "ApplicationProtocolPacket.h"
#include "ApplicationProtocolDef.h"

// APPL_TYPE_R_TRY_CONNECT_TO_SERVER
class TR_Try_Connect_To_Server : public TBasePacket
{
	// ushort type,ushort mVersion,char* nick
	unsigned short mVersion;

public:
	TR_Try_Connect_To_Server(){mType=APPL_TYPE_R_TRY_CONNECT_TO_SERVER;mVersion=VERSION_PROGRAMM;};

	unsigned short getVersion(){return *(getPointerVersion());};
	unsigned short* getPointerVersion(){return (unsigned short*)(mData+sizeof(mType));}; 
	void setVersion(){*(getPointerVersion())=mVersion;};


	char* getNick(){return mData+sizeof(mType)+sizeof(mVersion);}; 

	void setNick(char*s)
	{
		mSize   = strlen(s)+1+sizeof(mType)+sizeof(mVersion);
		free(mData);
		mData = (char*)malloc(mSize);
		strcpy(getNick(),s);
		setType();
		setVersion();
	};
};
//-----------------------------------------------------------------------------
//APPL_TYPE_R_IN_FIGHT
class TR_In_Fight : public TBasePacket
{
	// ushort type
public:
	TR_In_Fight()
	{
		mType=APPL_TYPE_R_IN_FIGHT;
		mSize=sizeof(mType);
		mData=(char*)malloc(mSize);
		setType();
	};
};
//-----------------------------------------------------------------------------
// APPL_TYPE_R_GET_LIST_TANK
class TR_Get_List_Tank : public TBasePacket
{
	// ushort type 
public:
	TR_Get_List_Tank()
  {
    mType=APPL_TYPE_R_GET_LIST_TANK;
    mSize=sizeof(mType);
    mData=(char*)malloc(mSize);
    setType();
  }

};
//-----------------------------------------------------------------------------
// APPL_TYPE_R_EXIT_WAIT
class TR_Exit_Wait : public TBasePacket
{
  // ushort type 
public:
  TR_Exit_Wait()
  {
    mType=APPL_TYPE_R_EXIT_WAIT;
    mSize=sizeof(mType);
    mData=(char*)malloc(mSize);
    setType();
  }
};
//-----------------------------------------------------------------------------
class TR_Correct_Packet : public TBasePacket
{
  // ushort type 
public:
  TR_Correct_Packet()
  {
    mType=APPL_TYPE_R_CORRECT_PACKET;
    mSize=sizeof(mType);
    mData=(char*)malloc(mSize);
    setType();
  }
};
//-------------------------------------
#endif

