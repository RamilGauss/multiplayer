/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef TransportProtocolPacketH
#define TransportProtocolPacketH

#include <stddef.h>
#include <set>
#include "ShareMisc.h"

namespace nsNetTransportStruct
{

#if defined( WIN32 )
#pragma pack(push, 1)
#endif

// базовые структуры
//-----------------------------------------------------------------------------
// заголовок пакета транспортного уровня
struct THeader
{
  unsigned char  type;   // K - квитанция, S - нет гарантии доставки, P - гарантия доставки, С-обмен циклическими номерами, нужен для синхронизации транспорта
  unsigned short cn_in;  // циклический номер для определения свежести при Rcv
  unsigned short cn_out; // циклический номер при Send
  unsigned char  cntTry;
	unsigned int time_ms;// для проверки необходимости отсылки неподтвержденного пакета
  TIP_Port ip_port_dst;  //
	TIP_Port ip_port_src;  //
  THeader()
  {
    cntTry = -1;
  };
};
//-----------------------------------------------------------------------------
// описание пакета, который будут отправлять, задание краткого описания заголовка
struct TShortDescPacket
{
  TIP_Port ip_port_dst;  //
  TIP_Port ip_port_src;  //
	void*    packet;
	int      size;
	TShortDescPacket(){size=0;packet = NULL;}
	~TShortDescPacket(){}
};
//-----------------------------------------------------------------------------
class TInfoConnect //: public TObject
{
	// информация по сокету
public:
	TInfoConnect(){init();}
	//unsigned int   ip;
	//unsigned short port;
	// для Stream
	unsigned short cn_in_s;     // циклический номер для определения свежести при Rcv
	unsigned short cn_out_s;    // циклический номер при Send
	// для Packet
	unsigned short cn_in_p;     // циклический номер для определения свежести при Rcv
	unsigned short cn_out_p;    // циклический номер при Send

	typedef std::set<unsigned short> TSetUshort;
	TSetUshort setIndexWaitRecv;// список пакетов, ЦН которых меньше текущего, но которые еще не были получены
	void init()
	{
		cn_in_s  = 0;
		cn_out_s = 0;
		cn_in_p  = 0;
		cn_out_p = 0;
		setIndexWaitRecv.clear();
	}
};
//-----------------------------------------------------------------------------

#if defined( WIN32 )
#pragma pack(pop)
#endif

}

#endif