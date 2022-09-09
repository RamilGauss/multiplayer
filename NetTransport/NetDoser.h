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


#ifndef NetDoserH
#define NetDoserH

#include "NetTransport.h"
#include "List.h"

// ���� ������:
// 1. �������� ������� (������� ������ �������� ����� �������� � �����������), �������� ������� ��������.
// 2. �������� ������� ������� 1.4�� - 1��.
// � ������ ����������� �����, ������� ��� ��������� � ������.
// ����� ������� ������ � ������ �������� � � ������ �������� ������ ������ ������������� �������

class TNetDoser : public INetTransport
{
  enum
  {
    eLimitSizePacket  = 1350,// ������������ ������ ������, ������� �� ������� ���������, ����
    eLimitCountPacket = 10, 
    // �����, ������ �������� ����� ���-�� �������, ������� ������ eLimitSizePacket
    eSizeWakeUpThread = eLimitCountPacket*eLimitSizePacket,// ����
    eWaitThread       = 50,// ��
    eSleepThread      = 60,

    // 
    eCntGroupSend     = 100, // 
    eTimeSleepBetweenGroup = 10,// ��
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

  TCallBackRegistrator mCallBackRecvPacket;// ��������� �� ip, port, ����� � ������ ������ �� ���������, ������ - {4�|2�|size�-6�}
  TCallBackRegistrator mCallBackRecvStream;// ��������� �� ip, port, ����� � ������ ������ �� ���������, ������ - {4�|2�|size�-6�}
  TCallBackRegistrator mCallBackDisconnect;// ��������� �� ip, port � ��� ��������� ������ �����

public:

	TNetDoser(char* pPathLog=NULL);
	virtual ~TNetDoser();
	virtual void InitLog(char* pPathLog);

  virtual bool Open(unsigned short port, unsigned char numNetWork = 0);

	virtual void Write(unsigned int ip, unsigned short port, void* packet, int size, bool check = true);

	// ������ - ���������������
  virtual void Register(TCallBackRegistrator::TCallBackFunc pFunc, int type);
  virtual void Unregister(TCallBackRegistrator::TCallBackFunc pFunc, int type);

	virtual void Start();
	virtual void Stop();

  // ���������� �������
  virtual bool Synchro(unsigned int ip, unsigned short port); // ����� ������ ��� �������

private:
	TNetTransport mTransport;
  //---------------------------------------------------------------
  // ��������� �������� ����� ������, ������� ����������
  struct THeaderDescPart
  {
    unsigned short cnt;// ���-�� ������ � ������
    unsigned short num;// ����� �����
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
  // ������� � ������ ���������� ������ �� �������� ������
  // �������� ������ �� ��������
  TList<TPacket> mListPacket; // ������, ������� � ������ ������ ���� ������������, ���� ���� �����

  volatile int mSumSizePacket;// ����� ���� ����, ������� ��������� ���������

protected:
  friend void* ThreadDoser(void*p);
  void Engine();

  int GetSizeTrySend();
  void AddPacket(unsigned int ip, unsigned short port, void* packet, int size, bool check);
  void RemovePacket(TPacket** pPacket);
  bool Send(TPacket* pPacket);
  void Work();

	bool TestConnection();// ������ ������

public:
  void RecvPacket(void* p, int s);
  void RecvStream(void* p, int s);
  void Disconnect(void* p, int s);
};


#endif