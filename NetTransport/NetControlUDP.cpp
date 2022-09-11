/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#include "NetControlUDP.h"
#include "BL_Debug.h"
#include "common_defs.h"
#include "INetMakerEvent.h"

using namespace std;

TNetControlUDP::TNetControlUDP()
{
	mSocketLocal = -1;

	mArrConnect.Sort(SortFreshInfoConnect);
	mReadSize = 0;
}
//------------------------------------------------------------------------------
TNetControlUDP::~TNetControlUDP()
{

}
//------------------------------------------------------------------------------
void TNetControlUDP::Work(int sock, list<eTypeEvent>& event)
{
  list<eTypeEvent>::iterator bit = event.begin();
  list<eTypeEvent>::iterator eit = event.end();
  while(bit!=eit)
  {
    switch(*bit)
    {
      case INetControl::eRead:
				ReadEvent();
        break;
      case INetControl::eWrite:
				WriteEvent();
        break;
      case INetControl::eConnect:
				ConnectEvent();
        break;
      case INetControl::eAccept:
				AcceptEvent();				
        break;
      case INetControl::eClose:
				CloseEvent();
        break;
    }
    bit++;
  }
}
//------------------------------------------------------------------------------
bool TNetControlUDP::Open( unsigned short port, unsigned char numNetWork )
{
	Close();
  mSocketLocal = mDevice.Open(false, port, numNetWork);

	GetMakerEvent()->Remove(mSocketLocal);
	GetMakerEvent()->Add(mSocketLocal, this);
	list<INetControl::eTypeEvent> lEvent;
	lEvent.push_back(INetControl::eRead);
	GetMakerEvent()->SetTypeEvent(mSocketLocal, lEvent);
  return (mSocketLocal!=-1);
}
//------------------------------------------------------------------------------
bool TNetControlUDP::Connect(unsigned int ip, unsigned short port)
{
	BL_MessageBug("Try connect on UDP!");
  return false;// ������, ������ TCP
}
//------------------------------------------------------------------------------
void TNetControlUDP::Send(unsigned int ip, unsigned short port, TBreakPacket& bp)
{
	// ��������� ���������
	TInfoConnect* pConnect = GetInfoConnect(TIP_Port(ip,port));
	unsigned short count_out = pConnect->cnt_out++;
  bp.PushFront( (char*)&count_out, sizeof(count_out));
	bp.Collect();
	mDevice.Send(mSocketLocal, (char*)bp.GetCollectPtr(), bp.GetSize(), ip, port);
}
//------------------------------------------------------------------------------
void TNetControlUDP::Close()
{
	mDevice.Close(mSocketLocal);
	mSocketLocal = -1;
}
//------------------------------------------------------------------------------
int TNetControlUDP::SortFreshInfoConnect(const void* p1, const void* p2)
{
	const TInfoConnect *s1 = *( const TInfoConnect **)p1;
	const TInfoConnect *s2 = *( const TInfoConnect **)p2;

	if(s1->ip>s2->ip)
		return -1;
	else 
		if(s1->ip==s2->ip)
		{
			if(s1->port>s2->port)
				return -1;
			else 
				if(s1->port==s2->port)
					return 0;
		}
		return 1;
}
//----------------------------------------------------------------------------------
bool TNetControlUDP::IsStreamFresh( TIP_Port& ip_port)
{
	TInfoConnect* pFoundFresh = GetInfoConnect(ip_port);

	unsigned short cnt_in = ((unsigned short*)mBuffer)[0];
	if(A_more_B( cnt_in, pFoundFresh->cnt_in))
	{
		pFoundFresh->cnt_in = cnt_in;
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------------
bool TNetControlUDP::A_more_B(unsigned short A, unsigned short B)
{
	return ::A_more_B_cycle<unsigned short>(A,B);
}
//----------------------------------------------------------------------------------
void TNetControlUDP::ReadEvent()
{
	unsigned int   ip;
	unsigned short port;
	mReadSize = mDevice.Read(mSocketLocal, mBuffer, sizeof(mBuffer), ip, port);
	TIP_Port ip_port(ip, port);
	if(IsStreamFresh(ip_port))
	{
		INetTransport::TDescRecv descRecv;
		descRecv.ip_port      = ip_port;
		descRecv.type					= INetTransport::eStream;
		descRecv.data         = mBuffer;
		descRecv.sizeData     = mReadSize;
		NotifyRecv((char*)&descRecv, sizeof(descRecv));
	}
}
//----------------------------------------------------------------------------------
void TNetControlUDP::WriteEvent()
{
	// �����
}
//----------------------------------------------------------------------------------
void TNetControlUDP::ConnectEvent()
{
	// �����
}
//----------------------------------------------------------------------------------
void TNetControlUDP::AcceptEvent()		
{
	// �����
}
//----------------------------------------------------------------------------------
void TNetControlUDP::CloseEvent()
{
	// �����
}
//----------------------------------------------------------------------------------
TNetControlUDP::TInfoConnect* TNetControlUDP::GetInfoConnect(TIP_Port& v)
{
	TInfoConnect fresh;
	TInfoConnect* pFresh = &fresh;
	pFresh->ip    = v.ip;
	pFresh->port  = v.port;

	lockSendRcv();
	TInfoConnect* pFoundFresh = (TInfoConnect*)mArrConnect.Get(mArrConnect.FastSearch(&pFresh,NULL,SortFreshInfoConnect));
	unlockSendRcv();
	if(pFoundFresh==NULL)
	{
		pFresh = new TInfoConnect;
		pFresh->ip    = v.ip;
		pFresh->port  = v.port;

		lockSendRcv();
		mArrConnect.Add(pFresh);
		unlockSendRcv();
		return pFresh;
	}

	return pFoundFresh;
}
//----------------------------------------------------------------------------------
