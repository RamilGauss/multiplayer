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


#ifndef DoserProtocolPacketH
#define DoserProtocolPacketH

#include <stddef.h>
#include <set>

#include "TObject.h"
#include "ShareMisc.h"
#include "List.h"
#include "AutomatRegularSpeed.h"
#include "DoserProtocolEnum.h"

#include "HiTimer.h"
#include "BreakPacket.h"

namespace nsNetDoser
{

#if defined( WIN32 )
#pragma pack(push, 1)
#endif

struct THeaderBasePacket
{
  unsigned char type;
};
//------------------------------------------------
struct THeaderOverload : public THeaderBasePacket
{
  // ������� �������� ���������, ��
  unsigned short over_ms;
  // ����� ���������� ����������� ������ (����������� � ���������� ������)
  unsigned int time_last_packet_ms;
  
  THeaderOverload()
  {
    type = eOverload;
  }
};
//------------------------------------------------
struct THeaderSinglePacket : public THeaderBasePacket
{
  unsigned int time_ms;// ��������� ����� ��������, ��
  THeaderSinglePacket()
  {
    type = eSinglePacket;
  }
  void SetTime()
  {
    time_ms = ht_GetMSCount();
  }
};
//------------------------------------------------
struct THeaderBigPacket : public THeaderSinglePacket
{
  unsigned short id; // id �������� ������
  unsigned short cnt;// ���-�� ������� � ����� ������� ������
  unsigned short num;// ����� � ������
  THeaderBigPacket()
  {
    type = eBigPacket;
  }
};
//------------------------------------------------
class TDescSendPacket
{
  TIP_Port ip_port;
  bool check;
  TBreakPacket mPacket;
  TContainer mContainer;
public:
  TDescSendPacket(TIP_Port &_ip_port, bool check, TBreakPacket& packet);
  ~TDescSendPacket(){Done();}
  void Done();
};
//------------------------------------------------
class TDescConnect : public TObject
{
public:

  TAutomatRegularSpeed* GetAutomat();
  unsigned int   GetIP()const;
  unsigned short GetPort()const;
  TIP_Port* GetIP_Port();
protected:
  // key
  TIP_Port mIP_port;
  // value
  TAutomatRegularSpeed mAutomatRegularSpeed;  // �������, ������� ����������� �������� ������
};
//------------------------------------------------

#if defined( WIN32 )
#pragma pack(pop)
#endif

}

#endif