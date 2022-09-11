
#include "PacketTransportLevel_SI.h"
#include "StandIndication_struct.h"

#define PRINTF_PTL mLog.WriteF_time

//--------------------------------------------------------------------------------------------
TPacketTransportLevel_SI::TPacketTransportLevel_SI()
{
	mLog.SetPrintf(true);
	mLog.SetBufferization(false);
//	mLog.SetEnable(false);	// выключить отладку
	//--------------------------------------------------
	mData_HEX    = NULL;
	mData_Packet = NULL;
	mSize_HEX    = 0;
}
//--------------------------------------------------------------------------------------------
TPacketTransportLevel_SI::~TPacketTransportLevel_SI()
{
	Done();
}
//--------------------------------------------------------------------------------------------
void TPacketTransportLevel_SI::Done()
{
	delete[]mData_Packet;
	mData_Packet = NULL;
	delete[]mData_HEX;
	mData_HEX    = NULL;
	mSize_HEX    = 0;
}
//--------------------------------------------------------------------------------------------		
void TPacketTransportLevel_SI::SetData(unsigned char* hex, int size)
{
	if(size==0)
	{
		PRINTF_PTL("TPacketTransportLevel_SI::SetData() size==0\n");
		return;
	}
	//------------------------------------------------------------------------------
	unsigned char sSizeANSCII[10];
	if(mSize_HEX!=size)
	{
		// создать контейнер
		Done();
		//выделить заново
		mSize_HEX    = size;
		unsigned char sizeData_ANSCII = GetSizePacket() - eSizeHeader - eSizeSize - eSizeCRC - eSizeEnd;
		unsigned char sizePacket_ANSCII = GetSizePacket();
		mData_HEX    = new char[mSize_HEX];
		mData_Packet = new char[sizePacket_ANSCII];
		// 1. Заголовок
		mData_Packet[0] = eHeader;
		//------------------------------------------------------------------------------
		// 2. Размер
		ConvertHEX2ANSCII(&sSizeANSCII[0],&sizeData_ANSCII,sizeof(sizeData_ANSCII));
		memcpy(&mData_Packet[eSizeHeader], &sSizeANSCII[0],sizeof(sizeData_ANSCII)*eSizeSize);
		//-----------------------------		
		// 3. Окончание
		mData_Packet[sizePacket_ANSCII-1] = eEnd;		
	}
	// 4. копировать данные во внутренний буфер и конвертировать
	memcpy(&mData_HEX[0],&hex[0],mSize_HEX);
	ConvertHEX2ANSCII((unsigned char*)&mData_Packet[eSizeHeader+eSizeSize],(unsigned char*)&mData_HEX[0], mSize_HEX);
	//------------------------------------------------------------------------------
  // 5. CRC
	int offCRC = GetSizePacket() - eSizeCRC - eSizeEnd;
	unsigned char crcPacket = CRC8((char*)&mData_Packet[0],offCRC);
	ConvertHEX2ANSCII((unsigned char*)&mData_Packet[offCRC],&crcPacket,sizeof(crcPacket));
	//------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------------		
int TPacketTransportLevel_SI::SetPacket(unsigned char* buffer, int size)// попытаться вставить пакет (вернет проверку на целостность пакета)
{
	// Проверки
	// 1. Заголовок
	if(buffer[0]!=eHeader)
	{
		PRINTF_PTL("TPacketTransportLevel_SI::SetPacket() packet is not full - eNotFoundHeader\n");
		return eNotFoundHeader;
	}
	//------------------------------------------------------------------------------
	// 2. По размеру
	if(size<eSizeWithoutData)
	{
		PRINTF_PTL("TPacketTransportLevel_SI::SetPacket() packet is not full - eSmallSize\n");
		return eSmallSize;
	}
	//------------------------------------------------------------------------------
	// 3. Ищем окончание
	int sizePacket = 0;
	bool resFoundEnd = false;
	for(int i = eSizeWithoutData-eSizeEnd ; i < size ; i++ )
	{
		if(buffer[i]==eEnd)
		{
			sizePacket = i+1;
			resFoundEnd = true;
			break;
		}
	}
	if(resFoundEnd==false)
	{
		if(size>eLimitSizePacket)
		{
			PRINTF_PTL("TPacketTransportLevel_SI::SetPacket() packet is not full - eSizeMoreMax\n");
			return eSizeMoreMax;
		}		
		PRINTF_PTL("TPacketTransportLevel_SI::SetPacket() packet is not full - eNotFoundEnd\n");
		return eNotFoundEnd;
	}
	//------------------------------------------------------------------------------
	// 4. Истинный размер
	unsigned char newSize_HEX;
	unsigned char sSizePacket[5];
	memcpy(&sSizePacket[0],&buffer[0+eSizeHeader],eSizeSize);
	ConvertANSCII2HEX(&newSize_HEX,&sSizePacket[0],eSizeSize/eByteInSize);
	if(newSize_HEX!=GetSizeHEX(sizePacket))
	{
		PRINTF_PTL("TPacketTransportLevel_SI::SetPacket() packet is not full - ePacketBreak\n");
		return ePacketBreak;
	}
	//------------------------------------------------------------------------------
	// 5. CRC
	unsigned char crc = CRC8((char*)buffer,newSize_HEX + eSizeHeader + eSizeSize);
	unsigned char crcPacket;
	ConvertANSCII2HEX(&crcPacket,&buffer[newSize_HEX + eSizeHeader + eSizeSize],eSizeCRC/eByteInSize);
	if(crc!=crcPacket)
	{
		PRINTF_PTL("TPacketTransportLevel_SI::SetPacket() packet is not full - ePacketBreak\n");
		return eCRC_fail;
	}
	//------------------------------------------------------------------------------
	// проверка закончена
	// 1. Выделить память если размер изменился
	if(newSize_HEX!=mSize_HEX)
	{
		Done();
		mSize_HEX    = newSize_HEX/eByteInSize;
		mData_HEX    = new char[mSize_HEX];
		mData_Packet = new char[GetSizePacket()];
	}
	//------------------------------------------------------------------------------
	// 2. сборка HEX
	ConvertANSCII2HEX((unsigned char*)&mData_HEX[0],&buffer[eSizeHeader+eSizeSize],mSize_HEX);
	// 3. Копировать пакет в буфер
	memcpy(&mData_Packet[0],&buffer[0],GetSizePacket());
	
	return eNormal;
}
//--------------------------------------------------------------------------------------------
const char* TPacketTransportLevel_SI::GetPacket(int& size) 
{
	size = GetSizePacket();
	return mData_Packet;
}
//--------------------------------------------------------------------------------------------
const char* TPacketTransportLevel_SI::GetData(int& size)
{
	size = mSize_HEX;
	return mData_HEX;
}
//--------------------------------------------------------------------------------------------
void TPacketTransportLevel_SI::ConvertHEX2ANSCII(unsigned char* dst,unsigned char* src,int cnt_src)
{
	for(int i = 0 ; i < cnt_src ; i++)
	{
		char str[10];
		unsigned char lb = src[i]&0x0F;// сначала старшая тетрада старшего байта
		unsigned char hb = src[i]&0xF0;// потом младшая тетрада старшего байта
		sprintf(str,"%x",hb>>4);
		memcpy(&dst[2*i+0],&str[0],1);
		sprintf(str,"%x",lb);				
		memcpy(&dst[2*i+1],&str[0],1);
	}
}
//--------------------------------------------------------------------------------------------
void TPacketTransportLevel_SI::ConvertANSCII2HEX(unsigned char* dst,unsigned char* src,int cnt_src)
{
	for(int i = 0 ; i < cnt_src ; i++)
	{
		unsigned char lb = hex2i((char)src[2*i+1]);
		unsigned char hb = hex2i((char)src[2*i+0]);		
		dst[i] = lb|(hb<<4);	
	}
}
//--------------------------------------------------------------------------------------------
int TPacketTransportLevel_SI::GetSizePacket() const
{
	return eByteInSize*mSize_HEX+eSizeWithoutData;
}
//--------------------------------------------------------------------------------------------
int TPacketTransportLevel_SI::GetSizeHEX(int sizePacket)
{
	return sizePacket-eSizeWithoutData;
}
//--------------------------------------------------------------------------------------------
unsigned char TPacketTransportLevel_SI::CRC8(char* buffer, int size)
{
	int crc = 0;
	for(int i = 0 ; i < size ; i++ )
		crc += buffer[i];

	return (crc%0xFF);
}
//--------------------------------------------------------------------------------------------