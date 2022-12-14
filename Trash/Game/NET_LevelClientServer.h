/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
??????? ?????? ????????? 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef NET_LevelClientServerH
#define NET_LevelClientServerH

#include "CallBackRegistrator.h"
#include "TransportProtocol.h"
#include "HiTimer.h"
#include "SrcEvent.h"

class TNET_LevelClientServer : public TSrcEvent
{
	bool flgNeedStop;
	bool flgActive;
	enum{eWaitThread = 100, // ??
			 eWaitCheck  = 1500, //??, ??????? ????? ????? 3 ???????
	};

	volatile guint32 mLastTimeRcv;// ????? ?????????? ????????? ?????? ?? ??????? (????? ??? ??????? ?????)

	TTransportProtocol mTransport;

	TCallBackRegistrator mCallBackDisconnect;
	TCallBackRegistrator mCallBackStream;
	TCallBackRegistrator mCallBackPacket;

	GThread* thread;// ????? ??????????? ??????????? ?? ???????

  unsigned int mIP_server;
public:
  
  TNET_LevelClientServer();
  virtual ~TNET_LevelClientServer();
  void start();
  void stop();
  void Register(TCallBackRegistrator::TCallBackFunc pFunc, int type);
  void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type);

//--------------------------------------------------------------------
public:
  bool IsConnect();
  
//--------------------------------------------------------------------
public:
  // ??????? ? ??????? ?? ??????
  // request
  bool Connect(unsigned int ip, unsigned int port, char* sNick);//

  void SendRequestListTank();
  void SendRequestExitFromWait();
  void SendRequestExitFromFight();
  void SendRequestInFight();
  // cmd
  void SetCurrentTank(int i);
  void PushButton(int button);

  void SendRequestCorrectPacket();
  
  void SendOrientAim(float x,float y,float z);
//--------------------------------------------------------------------
protected:
	friend void* ClientThreadDefDisconnect(void*p);
	void startThreadDefDisconnect();
	void ThreadDefDisconnect();

	friend void ClientCallBackRcvPacket(void* data, int size);
	friend void ClientCallBackRcvStream(void* data, int size);
	friend void ClientCallBackRcvDisconnect(void* data, int size);
 	
	// ??? ??????????? ??????? ?????????????????? ???????? (??? ??????? GUI ????)
	void RcvStream(void* data, int size);	
	void RcvPacket(void* data, int size);
	void Disconnect(void* data, int size);	

	void SetLastTime(){mLastTimeRcv=ht_GetMSCount();};
	guint32 GetLastTime(){return mLastTimeRcv;};


  void WriteTransport(void* packet);
  void WriteStream(void* packet);
};


#endif