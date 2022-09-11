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


#include "NetControlAcceptor.h"
#include "BL_Debug.h"

#include "Logger.h"
#include "NetTransport_Boost.h"
#include "NetControlTCP.h"
#include <boost\asio\placeholders.hpp>
#include <boost\system\error_code.hpp>

using namespace std;

TNetControlAcceptor::TNetControlAcceptor(boost::asio::io_service& io_service):
mDevice(io_service)
{
  pNewControlTCP = NULL;
}
//------------------------------------------------------------------------------
TNetControlAcceptor::~TNetControlAcceptor()
{
}
//------------------------------------------------------------------------------
bool TNetControlAcceptor::Open( unsigned short port, unsigned char numNetWork)
{
  return mDevice.Open(port,numNetWork);
}
//------------------------------------------------------------------------------
bool TNetControlAcceptor::Connect(unsigned int ip, unsigned short port)
{
  return false;
}
//------------------------------------------------------------------------------
void TNetControlAcceptor::Send(unsigned int ip, unsigned short port, TBreakPacket bp)
{

}
//------------------------------------------------------------------------------
void TNetControlAcceptor::Close()
{
  mDevice.Close();
}
//------------------------------------------------------------------------------
void TNetControlAcceptor::AcceptEvent(const boost::system::error_code& error)
{
  if(error==0)
  {
    TIP_Port ip_port;
    ip_port.port = pNewControlTCP->GetDevice()->GetSocket()->remote_endpoint().port();
    ip_port.ip   = pNewControlTCP->GetDevice()->GetSocket()->remote_endpoint().address().to_v4().to_ulong();
    pNewControlTCP->GetDevice()->SetIP_Port(ip_port);
    GetNetBoost()->AddInMapTCP(ip_port,pNewControlTCP);
    pNewControlTCP->Init();// готов к чтению
  }
  else
	{
		delete pNewControlTCP;
		pNewControlTCP = NULL;
		GetLogger()->Get(STR_NAME_NET_TRANSPORT)->
      WriteF_time("Acceptor AcceptEvent FAIL: %s.\n", error.message().data());
		return;
	}

  ReadyAccept();
}
//------------------------------------------------------------------------------
void TNetControlAcceptor::Done()
{

}
//------------------------------------------------------------------------------
void TNetControlAcceptor::ReadyAccept()
{
  pNewControlTCP = new TNetControlTCP(mDevice.GetSocket()->get_io_service());
  mDevice.GetSocket()->async_accept(*(pNewControlTCP->GetDevice()->GetSocket()),
    boost::bind(&TNetControlAcceptor::AcceptEvent, this, 
    boost::asio::placeholders::error));
}
//------------------------------------------------------------------------------
void TNetControlAcceptor::Init()
{
  ReadyAccept();
}
//------------------------------------------------------------------------------
