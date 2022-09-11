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

#include "Master.h"

namespace nsMelissa
{

TMaster::TMaster()
{

}
//-------------------------------------------------------------------------
TMaster::~TMaster()
{

}
//-------------------------------------------------------------------------
unsigned int TMaster::TryCreateGroup(std::list<unsigned int>& l)
{
	return (unsigned int)(-1);
}
//-------------------------------------------------------------------------
void TMaster::DestroyGroup(unsigned int id_group)
{

}
//-------------------------------------------------------------------------
void TMaster::LeaveGroup(unsigned int id_client)
{

}
//-------------------------------------------------------------------------
void TMaster::GetListForGroup(unsigned int id_group, std::list<unsigned int>& l)
{

}
//-------------------------------------------------------------------------
void TMaster::SetResultLogin(bool res, unsigned int id_session, 
                    unsigned int id_client, 
                    void* resForClient, int sizeResClient)
{

}
//-------------------------------------------------------------------------
unsigned int TMaster::GetSlaveSessionByGroup(unsigned int id_group)
{
	return INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------------------
void TMaster::SendToClient(unsigned int id_client, void* data, int size, bool check )
{

}
//-------------------------------------------------------------------------
void TMaster::Work()
{

}
//-------------------------------------------------------------------------
void TMaster::Disconnect(unsigned int id_session)
{

}
//-------------------------------------------------------------------------
int TMaster::GetCountDown()
{
	return 0;
}
//-------------------------------------------------------------------------
bool TMaster::GetDescDown(int index, void* pDesc, int& sizeDesc)
{
	return false;
}
//-------------------------------------------------------------------------
void TMaster::RecvFromClient(TDescRecvSession* pDesc)
{
  
}
//-------------------------------------------------------------------------
void TMaster::RecvFromSlave(TDescRecvSession* pDesc)
{
  
}
//-------------------------------------------------------------------------
void TMaster::RecvFromSuperServer(TDescRecvSession* pDesc)
{
  
}
//-------------------------------------------------------------------------

}