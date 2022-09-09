#ifndef StandIndication_StructH
#define StandIndication_StructH

#ifdef WIN32
  #include "ShareMisc.h"
#else
  #include "../../Share/ShareMisc.h"
#endif

#define VERSION_SI 0x1

namespace nsStandIndication
{
	//---------------------------------------------------------------------------------
	struct TPacketSet
	{
		unsigned char isPRM         : 1; // 0–ПРМ1, 1-ПРМ 2
		unsigned char isEmission    : 1; // 0–отсутсвие, 1-наличие излучения
		unsigned char ist12         : 1; // 1 - отказ вент.1 СЖО
		unsigned char ist13         : 1; // 1 - отказ вент.2 СЖО
		
		unsigned char ist14         : 1; // 1 - отказ насоса СЖО
		unsigned char um1           : 1; // 1 отказ УМ1 (обобщенный)
		unsigned char um2           : 1; // 1 отказ УМ2 (обобщенный)
		unsigned char um3           : 1; // 1 отказ УМ3 (обобщенный)
		//-----------------------------------------------------------------------
		unsigned char um4           : 1; // 1 отказ УМ4 (обобщенный)
		unsigned char um5           : 1; // 1 отказ УМ5 (обобщенный)
		unsigned char um6           : 1; // 1 отказ УМ6 (обобщенный)
		unsigned char irp1          : 1; // 1 - индикатор работы передатчика 1
		         
		unsigned char irp2          : 1; // 1 -                              2
		unsigned char irp3          : 1; // 1 -                              3
		unsigned char irp4          : 1; // 1 -                              4
		unsigned char irp5          : 1; // 1 -                              5
		//-----------------------------------------------------------------------
		unsigned char irp6          : 1; // 1 индикатор работы передатчика 6
		unsigned char afu1          : 1; // 1 отказ АФУ УМ 1
		unsigned char afu2          : 1; // 1 отказ АФУ УМ 2
		unsigned char afu3          : 1; // 1 отказ АФУ УМ 3
		        
		unsigned char afu4          : 1; // 1 отказ АФУ УМ 4
		unsigned char afu5          : 1; // 1 отказ АФУ УМ 5
		unsigned char afu6          : 1; // 1 отказ АФУ УМ 6
		unsigned char lit1          : 1; // 1 - индикатор работы передатчика 1
		unsigned char lit2          : 1; // 1 -                              2

		unsigned char lit3          : 1; // 1 -                              3
		unsigned char lit4          : 1; // 1 -                              4
		unsigned char lit5          : 1; // 1 -                              5
		unsigned char lit6          : 1; // 1 индикатор работы передатчика 6
	}_PACK;
	//---------------------------------------------------------------------------------	
	struct TPacketAnswerState
	{
		unsigned char isAPDS            : 1;//0–отсутсвие,1-наличие АПДС
		unsigned char isBlockPreocessing: 1;//0–отсутсвие,1-наличие Блока обработки
		unsigned char isVKIO            : 1;//0–отсутсвие,1-наличие ВКИО
		unsigned char is208             : 1;//0–отсутсвие,1-наличие прибора 208
	}_PACK;
	//---------------------------------------------------------------------------------	
	struct TPacketAnswerPrd
	{
		unsigned char isPrd2            : 1;//0–прд 1,1- прд 2
	}_PACK;
	//---------------------------------------------------------------------------------	
	struct TPacketAnswerStateSystemCool
	{
		unsigned char isOk             : 1;//0–,1-
	}_PACK;
	//---------------------------------------------------------------------------------	
  struct TPacketAnswerVersion
  {
    unsigned char version;
    TPacketAnswerVersion()
    {
      version = VERSION_SI;
    }
  }_PACK;
  //---------------------------------------------------------------------------------	
}

inline unsigned char hex2i(char s)
{
	switch(s)
	{
		case '0':
			return 0x0;
		case '1':
			return 0x1;
		case '2':
			return 0x2;
		case '3':
			return 0x3;
		case '4':
			return 0x4;
		case '5':
			return 0x5;
		case '6':
			return 0x6;
		case '7':
			return 0x7;
		case '8':
			return 0x8;
		case '9':
			return 0x9;
		case 'A':
		case 'a':
			return 0xA;
		case 'B':
		case 'b':
			return 0xB;
		case 'C':
		case 'c':
			return 0xC;
		case 'D':
		case 'd':
			return 0xD;
		case 'E':
		case 'e':
			return 0xE;
		case 'F':
		case 'f':
			return 0xF;
	}
	return 0;
}


#endif

