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

#include "NetTransport_Boost.h"

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/foreach.hpp>

#include "common_defs.h"
#include "HiTimer.h"
#include "NetSystem.h"
#include "Logger.h"


using namespace std;
using namespace boost::asio;

//----------------------------------------------------------------------------
TNetTransport_Boost::TNetTransport_Boost():
mNetWorkThread(),
mUDP(*(mNetWorkThread.GetIO_Service())),
mAcceptor(*(mNetWorkThread.GetIO_Service()))
{
  INetControl::SetNetBoost(this);

  pTCP_Up = new TNetControlTCP(*(mNetWorkThread.GetIO_Service()));
}
//----------------------------------------------------------------------------------
TNetTransport_Boost::~TNetTransport_Boost()
{
  Done();
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Done()
{

}
//----------------------------------------------------------------------------------
bool TNetTransport_Boost::Open(unsigned short port, unsigned char numNetWork)
{
  bool res = true;
  res &= pTCP_Up->Open(port, numNetWork);
  res &= mUDP.Open(port, numNetWork);
  if(res) mUDP.Init();
  res &= mAcceptor.Open(port, numNetWork);
  if(res) mAcceptor.Init();

  return res;
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Send(unsigned int ip, unsigned short port, 
							                   TBreakPacket packet, bool check)
{
  mMutexSend.lock();
  if(check)
  {
    mMutexMapIP_TCP.lock();
    //---------------------
    TNetControlTCP* pControl = GetTCP_ByIP(TIP_Port(ip,port));
    if(pControl)
      pControl->Send(ip,port,packet);
    //---------------------
    mMutexMapIP_TCP.unlock();
  }
  else
    mUDP.Send(ip, port, packet);
  mMutexSend.unlock();
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Start()
{
  mNetWorkThread.Start();
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Stop()
{
  CloseAll();

  mNetWorkThread.Stop();

  DeleteMapControlTCP();
}
//----------------------------------------------------------------------------------
bool TNetTransport_Boost::IsActive()
{
	return mNetWorkThread.IsActive();
}
//--------------------------------------------------------------------------
bool TNetTransport_Boost::Connect(unsigned int ip, unsigned short port)
{
  bool res = pTCP_Up->Connect(ip, port);
  if(res)
  {
    TIP_Port ip_port(ip, port);
		pTCP_Up->GetDevice()->SetIP_Port(ip_port);
    pTCP_Up->Init();
    // в boost другой порядок байт в описании порта и ip, поэтому пользоваться GetIP
    AddInMapTCP(ip_port,pTCP_Up);
  }
  return res;
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::Close(unsigned int ip, unsigned short port)
{
  mMutexMapIP_TCP.lock();
  //---------------------
  TNetControlTCP* pControl = GetTCP_ByIP(TIP_Port(ip,port));
  if(pControl)
    pControl->Close();
  //---------------------
  mMutexMapIP_TCP.unlock();
}
//----------------------------------------------------------------------------------
TNetControlTCP* TNetTransport_Boost::GetTCP_ByIP(TIP_Port &ip_port)
{
  TMapIP_PtrIt fit = mMapIP_TCP.find(ip_port);
  if(fit==mMapIP_TCP.end())
    return NULL;
  return fit->second;
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::AddInMapTCP(TIP_Port& ip_port, TNetControlTCP* pNetControl)
{
  mMutexMapIP_TCP.lock();
  //---------------------
    mMapIP_TCP.insert(TMapIP_Ptr::value_type(ip_port, pNetControl));
  //---------------------
  mMutexMapIP_TCP.unlock();
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::RemoveFromMapTCP(TIP_Port* ip_port, TNetControlTCP* pControl)
{
  mMutexMapIP_TCP.lock();
  //---------------------
    mMapIP_TCP.erase(*ip_port);
    delete pControl;
  //---------------------
  mMutexMapIP_TCP.unlock();
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::CloseAll()
{
  mUDP.Close();
  mAcceptor.Close();

  mMutexMapIP_TCP.lock();
  //---------------------
	BOOST_FOREACH( TMapIP_Ptr::value_type &bit, mMapIP_TCP )
    bit.second->Close();
  //---------------------
  mMutexMapIP_TCP.unlock();
}
//----------------------------------------------------------------------------------
void TNetTransport_Boost::DeleteMapControlTCP()
{
  BOOST_FOREACH( TMapIP_Ptr::value_type &bit, mMapIP_TCP )
    delete bit.second;
  
  mMapIP_TCP.clear();
}
//----------------------------------------------------------------------------------
