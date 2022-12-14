/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
??????? ?????? ????????? 
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

#include <winsock.h>

#include "INetMakerEvent.h"

INetMakerEvent::INetMakerEvent()
{

}
//--------------------------------------------------------------------------
INetMakerEvent::~INetMakerEvent()
{

}
//--------------------------------------------------------------------------
void INetMakerEvent::AddIPSock(TIP_Port& ip_port, int sock)
{
  lockIPSock();
  mMapDIPSock.insert( bmIPInt::value_type(ip_port, sock) );
  unlockIPSock();
}
//----------------------------------------------------------------------------------
void INetMakerEvent::RemoveSockIP(int sock)
{
  lockIPSock();
  mMapDIPSock.right.erase(sock);
  unlockIPSock();
}
//----------------------------------------------------------------------------------
void INetMakerEvent::RemoveIPSock(TIP_Port& ip_port)
{
  lockIPSock();
  mMapDIPSock.left.erase(ip_port);
  unlockIPSock();
}
//----------------------------------------------------------------------------------
int INetMakerEvent::GetSocketByIpPort(TIP_Port& ip_port)
{
  int sock = INVALID_SOCKET;
  lockIPSock();
  const bmIPInt::left_iterator fit = mMapDIPSock.left.find(ip_port);
  if(fit!=mMapDIPSock.left.end())
    sock = fit->second;
  unlockIPSock();
  return sock;
}
//----------------------------------------------------------------------------------
bool INetMakerEvent::GetIpPortBySocket(TIP_Port& ip_port, int sock)
{
  bool res = false;
  lockIPSock();
  const bmIPInt::right_iterator fit = mMapDIPSock.right.find(sock);
  if(fit!=mMapDIPSock.right.end())
  {
    ip_port = fit->second;
    res = true;
  }
  unlockIPSock();
  return res;
}
//--------------------------------------------------------------------------
