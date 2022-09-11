/*
			04.10.2012
				Gauss
	����� ��� ���������� � �������������� �� ������� ���������.
	����� ������������� ������� - �������� � ���������� ������ � ������������ � ����������� ����������
	��������� ��� ��������������� �� HEX � ANSCII
	�������� ������ �� "������"
*/

#ifndef PacketTransportLevel_SIH
#define PacketTransportLevel_SIH

#include "SaveOnHDD.h"

#include <string.h>
#include <stdio.h>
#include <vector>

class TPacketTransportLevel_SI
{
	TSaveOnHDD mLog;
protected:
	unsigned char mSize_HEX;// ANSCII ������ � eByteInSize ��� ������
	
	char* mData_Packet;
	char* mData_HEX;
	
	//---------------------------------------------------------------		
	enum{	
				eHeader     = 0x3A,
				eEnd        = 0x0D,
				
				// ����
				eSizeHeader = 1,
				eSizeSize   = 2,// ������ ���� ����, �� �����������?
				eSizeCRC    = 2,
				eSizeEnd    = 1,
				eByteInSize = eSizeSize/sizeof(char),
				eSizeWithoutData=eSizeHeader+eByteInSize+eSizeCRC+eSizeEnd,
				eLimitSizePacket = 250,
			};
public:
	TPacketTransportLevel_SI();
	virtual ~TPacketTransportLevel_SI();
	
	// ���� ������
	void SetData(  unsigned char* hex,    int size);// ������ ������ �������� ����� � ������
	
	enum{
		eNotFoundEnd = 0, // ��� ��������� 
		ePacketBreak,     // ����� ���������
		eNotFoundHeader,  // ��� ���������
		eSmallSize,       // ���� ������
		eCRC_fail,        // ����������� ����� �� ���������
		eSizeMoreMax,     // �� ��� ������ ����� ������, � ���������� ����� ������� �����
		eNormal,          // ����� ������
	};
	int SetPacket(unsigned char* buffer, int size);// ���������� �������� ����� (������ �������� �� ����������� ������)
	
	// ����� ������
	const char* GetPacket(int& size);
	const char* GetData(  int& size);
	
	int GetSizePacket() const;
	
protected:

	void ConvertHEX2ANSCII(unsigned char* dst,unsigned char* src, int size_src);
	void ConvertANSCII2HEX(unsigned char* dst,unsigned char* src, int size_src);	
	void Done();                           
	
	int GetSizeHEX(int sizePacket);// ������ ���� ������, ����
	
	unsigned char CRC8(char* buffer, int size);
};
  
#endif
