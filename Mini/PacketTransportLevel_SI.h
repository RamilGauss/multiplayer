/*
			04.10.2012
				Gauss
	 ласс дл€ управлени€ и взаимодействи€ со стендом индикации.
	ѕакет транспортного уровен€ - упаковка и распаковка пакета в соответствии с траспортным протоколом
	 онтейнер дл€ конвертировани€ из HEX в ANSCII
	явл€етс€ частью ѕќ "ћагний"
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
	unsigned char mSize_HEX;// ANSCII всегда в eByteInSize раз больше
	
	char* mData_Packet;
	char* mData_HEX;
	
	//---------------------------------------------------------------		
	enum{	
				eHeader     = 0x3A,
				eEnd        = 0x0D,
				
				// байт
				eSizeHeader = 1,
				eSizeSize   = 2,// размер пол€ сайз, че непон€тного?
				eSizeCRC    = 2,
				eSizeEnd    = 1,
				eByteInSize = eSizeSize/sizeof(char),
				eSizeWithoutData=eSizeHeader+eByteInSize+eSizeCRC+eSizeEnd,
				eLimitSizePacket = 250,
			};
public:
	TPacketTransportLevel_SI();
	virtual ~TPacketTransportLevel_SI();
	
	// ввод данных
	void SetData(  unsigned char* hex,    int size);// теперь объект содержит пакет и данные
	
	enum{
		eNotFoundEnd = 0, // нет окончани€ 
		ePacketBreak,     // пакет поврежден
		eNotFoundHeader,  // нет заголовка
		eSmallSize,       // мало данных
		eCRC_fail,        // контрольна€ сумма не совпадает
		eSizeMoreMax,     // не был найден конец пакета, а переданный буфер слишком велик
		eNormal,          // пакет собран
	};
	int SetPacket(unsigned char* buffer, int size);// попытатьс€ вставить пакет (вернет проверку на целостность пакета)
	
	// вывод данных
	const char* GetPacket(int& size);
	const char* GetData(  int& size);
	
	int GetSizePacket() const;
	
protected:

	void ConvertHEX2ANSCII(unsigned char* dst,unsigned char* src, int size_src);
	void ConvertANSCII2HEX(unsigned char* dst,unsigned char* src, int size_src);	
	void Done();                           
	
	int GetSizeHEX(int sizePacket);// размер пол€ данные, байт
	
	unsigned char CRC8(char* buffer, int size);
};
  
#endif
