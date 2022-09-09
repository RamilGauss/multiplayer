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

#ifndef WRAPPER_TRANSPORT_H
#define WRAPPER_TRANSPORT_H

#include "..\Melissa\ITransport.h"

class TWrapperTransport : public nsMelissa::ITransport
{

public:
	TWrapperTransport();
	virtual ~TWrapperTransport();
	//���� callback �������
	virtual void InitLog(char* pPathLog);
	virtual bool Open(unsigned char subNet, unsigned short port );
	virtual void Write(unsigned int ip, unsigned short port, void* packet, int size, bool check = true);
	virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, int type);
	virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type);
	virtual void Start();
	virtual void Stop();
	virtual bool Synchro(unsigned int ip, unsigned short port); // ����� ������ ��� �������

protected:
private:

};

#endif