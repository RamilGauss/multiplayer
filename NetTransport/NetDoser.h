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


#ifndef NetDoserH
#define NetDoserH

#include "NetTransport.h"
#include "List.h"

// цель класса:
// 1. Контроль трафика (слишком частая отправка может привести к дисконнекту), сгладить высокую нагрузку.
// 2. Отправка пакетов размера 1.4кб - 1Мб.
// В классе запускается поток, который тут переходит в спячку.
// Поток будится только в случае останова и в случае передачи пакета больше определенного размера

class TNetDoser : public INetTransport
{
  enum
  {
    eLimitSizePacket  = 1350,// максимальный размер пакета, который не требует дробления, байт
    eLimitCountPacket = 10, 
    // пакет, размер которого равно кол-ву пакетов, имеющих размер eLimitSizePacket
    eSizeWakeUpThread = eLimitCountPacket*eLimitSizePacket,// байт
    eWaitThread       = 50,// мс
    eSleepThread      = 60,

    // 
    eCntGroupSend     = 100, // 
    eTimeSleepBetweenGroup = 10,// мс
  };

	typedef enum
	{
		eTest         = 'T',
		eAnswerTest   = 'A',
		eResultTest   = 'R',
		eBigPacket    = 'D',
		eSinglePacket = 'S',
	}eTypePacket;

  bool flgActive;
  bool flgNeedStop;
	
  GThread* thread;

  TCallBackRegistrator mCallBackRecvPacket;// указатель на ip, port, пакет и размер данных по указателю, данные - {4б|2б|sizeб-6б}
  TCallBackRegistrator mCallBackRecvStream;// указатель на ip, port, пакет и размер данных по указателю, данные - {4б|2б|sizeб-6б}
  TCallBackRegistrator mCallBackDisconnect;// указатель на ip, port с кем произошел разрыв связи

public:

	TNetDoser(char* pPathLog=NULL);
	virtual ~TNetDoser();
	virtual void InitLog(char* pPathLog);

  virtual bool Open(unsigned short port, unsigned char numNetWork = 0);

	virtual void Write(unsigned int ip, unsigned short port, void* packet, int size, bool check = true);

	// чтение - зарегистрируйся
  virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, int type);
  virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type);

	virtual void Start();
	virtual void Stop();

  // синхронная функция
  virtual bool Synchro(unsigned int ip, unsigned short port); // вызов только для клиента

private:
	TNetTransport mTransport;
  //---------------------------------------------------------------
  // заголовок описания части пакета, который раздробили
  struct THeaderDescPart
  {
    unsigned short cnt;// кол-во частей в пакете
    unsigned short num;// номер части
  };
  //---------------------------------------------------------------
  struct TPacket
  {
    unsigned int ip;
    unsigned short port;
    void* packet;
    int size;
    bool check;
    TPacket(unsigned int ip, unsigned short port, void* packet, int size, bool check);
    ~TPacket(){Done();}
    void Done();
  };
  //---------------------------------------------------------------
  // добавка в список происходит только из внешнего потока
  // удаление только из рабочего
  TList<TPacket> mListPacket; // пакеты, которые в данный момент либо отправляются, либо ждут этого

  volatile int mSumSizePacket;// сумма всех байт, которые требуется отправить

protected:
  friend void* ThreadDoser(void*p);
  void Engine();

  int GetSizeTrySend();
  void AddPacket(unsigned int ip, unsigned short port, void* packet, int size, bool check);
  void RemovePacket(TPacket** pPacket);
  bool Send(TPacket* pPacket);
  void Work();

	bool TestConnection();// только клиент

public:
  void RecvPacket(void* p, int s);
  void RecvStream(void* p, int s);
  void Disconnect(void* p, int s);
};


#endif