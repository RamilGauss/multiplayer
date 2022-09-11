#include "PacketApplLevel_SI.h"

#include <stdio.h>
//#include <termios.h>
//#include <unistd.h>
//#include <sys/ioctl.h>
//#include <asm/ioctls.h>
//#include <sys/types.h>
//#include <sys/time.h>
//#include <fcntl.h>
#include <string.h>


TPacketApplLevel_SI::TPacketApplLevel_SI()
{
	mPacket = NULL;
	mSize = 0;
}
//-------------------------------------------------------------------------------------
TPacketApplLevel_SI::~TPacketApplLevel_SI()
{
	Done();
}
//-------------------------------------------------------------------------------------
void TPacketApplLevel_SI::SetData(void* data, int size, unsigned char type)
{
	if(size+eSizeType!=mSize)
	{
		Done();
		mSize = size+eSizeType;
		mPacket = new char[mSize];
	}
	memcpy(mPacket,&type,eSizeType);	
	memcpy(mPacket+eSizeType,data,size);
}
//-------------------------------------------------------------------------------------
void TPacketApplLevel_SI::SetPacket(void* packet, int size)
{
	if(size+eSizeType!=mSize)
	{
		Done();
		mSize = size;
		mPacket = new char[mSize];
	}
	memcpy(mPacket,packet,mSize);	
}
//-------------------------------------------------------------------------------------
char* TPacketApplLevel_SI::GetData() const
{
	return mPacket+eSizeType;
}
//-------------------------------------------------------------------------------------
char* TPacketApplLevel_SI::GetPacket() const	
{
	return mPacket;
}
//-------------------------------------------------------------------------------------
int TPacketApplLevel_SI::GetSizeData() const
{
	return mSize-eSizeType;
}
//-------------------------------------------------------------------------------------
int TPacketApplLevel_SI::GetSizePacket() const
{
	return mSize;
}
//-------------------------------------------------------------------------------------
void TPacketApplLevel_SI::Done()
{
	delete[]mPacket;
	mPacket = NULL;
	mSize = 0;
}
//-------------------------------------------------------------------------------------
unsigned char TPacketApplLevel_SI::GetType()const
{
	return *((unsigned char*)mPacket);
}
//-------------------------------------------------------------------------------------
