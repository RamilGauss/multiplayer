/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
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

// ������� ���������
//-----------------------------------------------------------------------------
// ��������� ������ ������������� ������
struct THeader
{
  unsigned char  type;   // K - ���������, S - ��� �������� ��������, P - �������� ��������, �-����� ������������ ��������, ����� ��� ������������� ����������
  unsigned short cn_in;  // ����������� ����� ��� ����������� �������� ��� Rcv
  unsigned short cn_out; // ����������� ����� ��� Send
  unsigned char  cntTry;
	unsigned int time_ms;// ��� �������� ������������� ������� ����������������� ������
  TIP_Port ip_port_dst;  //
	TIP_Port ip_port_src;  //
  THeader()
  {
    cntTry = -1;
  };
};
//-----------------------------------------------------------------------------
// �������� ������, ������� ����� ����������, ������� �������� �������� ���������
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
	// ���������� �� ������
public:
	TInfoConnect(){init();}
	//unsigned int   ip;
	//unsigned short port;
	// ��� Stream
	unsigned short cn_in_s;     // ����������� ����� ��� ����������� �������� ��� Rcv
	unsigned short cn_out_s;    // ����������� ����� ��� Send
	// ��� Packet
	unsigned short cn_in_p;     // ����������� ����� ��� ����������� �������� ��� Rcv
	unsigned short cn_out_p;    // ����������� ����� ��� Send

	typedef std::set<unsigned short> TSetUshort;
	TSetUshort setIndexWaitRecv;// ������ �������, �� ������� ������ ��������, �� ������� ��� �� ���� ��������
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