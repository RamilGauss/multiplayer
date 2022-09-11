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


#ifndef AutomatRegularSpeedH
#define AutomatRegularSpeedH

#include <list>

#include "DescSendPacket.h"
#include "StateARS.h"


/*
	Not Thread Safe
*/

class INetTransport;

namespace nsNetDoser
{

class TAutomatRegularSpeed
{
  TIP_Port mIP_Port;
  INetTransport* mTransport;

  enum{
       eDefTimeSleep = 18,// мс
    };
  // пакеты на отправку
  struct TReadyPacket
  {
    TDescSendPacket d;
    TContainer      collectPacket;

    void Init();
  };

  typedef std::list<TReadyPacket> TListReadyPacket;
  typedef TListReadyPacket::iterator TListReadyPacketIt;
  TListReadyPacket mListPacket; 

  TStateARS mStateARS;

public:
  TAutomatRegularSpeed(TIP_Port* ip_port);
  ~TAutomatRegularSpeed();

  void SetTransport(INetTransport* pTransport);// +
  // ведет статистику по времени опроса
  // например если опрос был 10 раз по 1350 байт в течение 1 секунды, то вернет 10 раз false
  // но если опрос был в течение 1 мс, то на 10 опрос вернет true
  // все зависит от настроек скорости в данный момент
  bool AddInQueue(int sizeBuffer);// добавлять ли в автомат пакеты, общий размер которых sizeBuffer -

  void Add(TDescSendPacket* pDescPacket);// добавить -

  bool NeedSend();// опрос, надо ли что-то отправлять -
  void Send();    // будет отсылать до тех пор пока не истечет время или не закончатся пакеты -

  void Overload(unsigned char cntTry);//unsigned short over_ms);// обратная связь с каналом -

protected:
};

}

#endif
