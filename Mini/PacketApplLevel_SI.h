#ifndef PacketApplLevel_SIH
#define PacketApplLevel_SIH

#include <string.h>
#include <stdio.h>
#include <vector>


class TPacketApplLevel_SI
{
	char* mPacket;
	int   mSize;
	
	enum{eSizeType=1,
	};
public:

	TPacketApplLevel_SI();
	~TPacketApplLevel_SI();	
		
	void SetData(void* data, int size, unsigned char type);
	void SetPacket(void* packet, int size);
	
	char* GetData() const;
	char* GetPacket() const;	
	
	unsigned char GetType()const;
	
	int GetSizeData() const;
	int GetSizePacket() const;	

protected:
	
	void TPacketApplLevel_SI::Done();
	
};

#endif


