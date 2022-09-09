/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef TransportProtocolPacketH
#define TransportProtocolPacketH

#include <stddef.h>
#include <set>
#include "TObject.h"

// квитанции K о получении высылаются если пришел REQUEST, ANSWER, COMMAND

// на STREAM квитанция не нужна 

#if defined( WIN32 )
#pragma pack(push, 1)
#endif

struct TIP_Port
{
  unsigned int ip;
  unsigned short port;
};
// базовые структуры
//-----------------------------------------------------------------------------
struct TPrefixTransport
{
  unsigned char  type;   // K - квитанция, S - нет гарантии доставки, P - гарантия доставки, С-обмен циклическими номерами, нужен для синхронизации транспорта
  unsigned short cn_in;  // циклический номер для определения свежести при Rcv
  unsigned short cn_out; // циклический номер при Send
  unsigned char  cntTry;
	unsigned int time_ms;// для проверки необходимости отсылки неподтвержденного пакета
  TIP_Port ip_port_dst;  //
	TIP_Port ip_port_src;  //
  TPrefixTransport()
  {
    cntTry = -1;
  };
};
//-----------------------------------------------------------------------------
struct InfoData
{
	unsigned int   ip_dst;     
	unsigned short port_dst;   
	unsigned int   ip_src;     
	unsigned short port_src;   
	void*          packet;
	int            size;
	InfoData(){size=0;packet = NULL;}
	~InfoData(){}
};
//-----------------------------------------------------------------------------
class  InfoConnect : public TObject
{
	// информация по сокету
public:
	InfoConnect(){init();}
	unsigned int   ip;
	unsigned short port;
	// для Stream
	unsigned short cn_in_s;     // циклический номер для определения свежести при Rcv
	unsigned short cn_out_s;    // циклический номер при Send
	// для Packet
	unsigned short cn_in_p;     // циклический номер для определения свежести при Rcv
	unsigned short cn_out_p;    // циклический номер при Send

	typedef std::set<unsigned short> TSetUshort;
	TSetUshort setIndexWaitRecv;// для определения свежести пакетов
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

#endif