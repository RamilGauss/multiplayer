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

#include "Slave.h"

namespace nsMelissa
{

TSlave::TSlave()
{

}
//-------------------------------------------------------------------------
TSlave::~TSlave()
{

}
//-------------------------------------------------------------------------
void TSlave::SaveContext(unsigned int id_session, void* data, int size)
{

}
//-------------------------------------------------------------------------
unsigned int TSlave::GetClientKeyBySession(unsigned int id_session)
{
	return (unsigned int)(-1);
}
//-------------------------------------------------------------------------
unsigned int TSlave::GetSessionByClientKey(unsigned int key)
{
  return INVALID_HANDLE_SESSION;
}
//-------------------------------------------------------------------------
void TSlave::DisconnectInherit(unsigned int id_session)
{

}
//-------------------------------------------------------------------------
int TSlave::GetCountDown()
{
	return 0;
}
//-------------------------------------------------------------------------
bool TSlave::GetDescDown(int index, void* pDesc, int& sizeDesc)
{
	return false;
}
//-------------------------------------------------------------------------
void TSlave::RecvFromClient(TDescRecvSession* pDesc)
{

}
//-------------------------------------------------------------------------
void TSlave::RecvFromMaster(TDescRecvSession* pDesc)
{

}
//-------------------------------------------------------------------------

}