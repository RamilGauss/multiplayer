/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "BasePacketNetTransport.h"

using namespace std;
using namespace nsNetTransportStruct;

const char* strPrefixTransport = "PrefixTransport";

TBasePacketNetTransport::TBasePacketNetTransport()
{
	Init();
}
//-------------------------------------------------------------------------
TBasePacketNetTransport::~TBasePacketNetTransport()
{

}
//-------------------------------------------------------------------------
int TBasePacketNetTransport::GetType()
{
	THeader v;
	GetPrefixTransport(v);
	return v.type;
}
//-------------------------------------------------------------------------
void TBasePacketNetTransport::Init()
{
	vector<TMarkUpContainer::TCommonDesc> vcd;
	TMarkUpContainer::TCommonDesc cd;
	// разметка заголовка пакета
	cd.type = TMarkUpContainer::eConst;
	cd.name = strPrefixTransport;
	cd.c.size = sizeof(THeader);
	vcd.push_back(cd);

	mC.SetMarkUp(&vcd);
	mC.Update();
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void TBasePacketNetTransport::SetHeader(TShortDescPacket* pInfoD )
{
	THeader v;
  v.ip_port_dst = pInfoD->ip_port_dst;
	v.ip_port_src = pInfoD->ip_port_src;
	v.cntTry           = -1;
	
	SetPrefixTransport(v);

	SetData(pInfoD->packet, pInfoD->size);
}
//-------------------------------------------------------------------------
void TBasePacketNetTransport::SetType(unsigned char type)
{
	GetPrefix()->type = type;
}
//-------------------------------------------------------------------------
void TBasePacketNetTransport::SetCnIn(unsigned short cn)
{
	GetPrefix()->cn_in = cn;
}
//-------------------------------------------------------------------------
void TBasePacketNetTransport::SetCnOut(unsigned short cn)
{
	GetPrefix()->cn_out = cn;
}
//-------------------------------------------------------------------------
void TBasePacketNetTransport::SetTime(unsigned int time_ms)
{
	GetPrefix()->time_ms = time_ms;
}
//-------------------------------------------------------------------------
void TBasePacketNetTransport::SetCntTry(unsigned char  cntTry)
{
	GetPrefix()->cntTry = cntTry;
}
//-------------------------------------------------------------------------
unsigned short TBasePacketNetTransport::GetCnIn()
{
	return GetPrefix()->cn_in;
}
//-------------------------------------------------------------------------
unsigned short TBasePacketNetTransport::GetCnOut()
{
	return GetPrefix()->cn_out;
}
//-------------------------------------------------------------------------
unsigned int TBasePacketNetTransport::GetTime()
{
	return GetPrefix()->time_ms;
}
//-------------------------------------------------------------------------
unsigned char TBasePacketNetTransport::GetCntTry()
{
	return GetPrefix()->cntTry;
}
//-------------------------------------------------------------------------
void* TBasePacketNetTransport::GetData(int & size, unsigned int& ip_dst, unsigned short& port_dst)
{
	size     = mC.GetSize();
	ip_dst   = GetPrefix()->ip_port_dst.ip;
	port_dst = GetPrefix()->ip_port_dst.port;
	return GetPrefix();
}
//-------------------------------------------------------------------------
unsigned int TBasePacketNetTransport::GetIP_dst() const
{
	return GetPrefix()->ip_port_dst.ip;
}
//-------------------------------------------------------------------------
unsigned short TBasePacketNetTransport::GetPort_dst() const
{
	return GetPrefix()->ip_port_dst.port;
}
//-------------------------------------------------------------------------
