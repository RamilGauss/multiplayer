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


#include "BasePacketNetTransport.h"

using namespace std;

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
	TPrefixTransport v;
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
	cd.c.size = sizeof(TPrefixTransport);
	vcd.push_back(cd);

	mC.SetMarkUp(&vcd);
	mC.Update();
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void TBasePacketNetTransport::SetHeader(InfoData* pInfoD )
{
	TPrefixTransport v;
  v.ip_port_dst.ip   = pInfoD->ip_dst;
	v.ip_port_dst.port = pInfoD->port_dst;
	v.ip_port_src.ip   = pInfoD->ip_src;
	v.ip_port_src.port = pInfoD->port_src;
	v.cntTry           = -1;
	
	//void* ppp = mC.Get("PrefixTransport");
	SetPrefixTransport(v);

	SetData(pInfoD->packet, pInfoD->size);

	//mInfoData = *pInfoD;
	//delete[]mReadyPacket;
	//mSizePacket = mInfoData.size+sizeof(TPrefixTransport);
	//mReadyPacket = new char[mSizePacket];
	//memcpy((char*)mReadyPacket+sizeof(TPrefixTransport),pInfoD->packet, mInfoData.size);
	//((TPrefixTransport*)mReadyPacket)->ip_port_dst.ip   = mInfoData.ip_dst;
	//((TPrefixTransport*)mReadyPacket)->ip_port_dst.port = mInfoData.port_dst;
	//((TPrefixTransport*)mReadyPacket)->ip_port_src.ip   = mInfoData.ip_src;
	//((TPrefixTransport*)mReadyPacket)->ip_port_src.port = mInfoData.port_src;
	//((TPrefixTransport*)mReadyPacket)->cntTry           = -1;
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
void TBasePacketNetTransport::SetIsQueue(bool v)
{
	isQueue = v;
}
//-------------------------------------------------------------------------
bool TBasePacketNetTransport::GetIsQueue()const
{
	return isQueue;
}
//-------------------------------------------------------------------------
