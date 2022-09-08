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
//APPL_TYPE_R_CORRECT_PACKET
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
//APPL_TYPE_R_EXIT_FIGHT
class TR_Exit_Fight : public TBasePacket
{
  // ushort type 
public:
  TR_Exit_Fight()
  {
    mType=APPL_TYPE_R_EXIT_FIGHT;
    mSize=sizeof(mType);
    mData=(char*)malloc(mSize);
    setType();
  }
};
//-------------------------------------

#endif

