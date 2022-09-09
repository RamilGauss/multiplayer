/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef RoomH
#define RoomH
#include "ServerStruct.h"
#include "TransportProtocol.h"
#include <list>
#include "glib/gthread.h"
#include "ManagerObjectCommon.h"
#include "Tank.h"

class TRoom : public TManagerObjectCommon
{

  //TPrediction mPrediction; - ������ ������� - TManagerObjectCommon

  TTransportProtocol* mTransport;

  TArrayObject mArrTank;// �������� ��������� �� TObject, �� ��-�� ������������ ������������ ������ ������� � TBaseObjectCommon, ������������ GetPtrInherits()

  guint32 mTimeBeginCountDown;// ������� �������� ������
  guint32 mTimeAfterCountDown;// ����� ��������� ��������� ������� (������� ���������� ��� ���)
  guint32 mNow_MS;

  bool flgEventReconnect;
  bool flgIsLoadingMap;
  GThread* threadLoadMap;

  int mState;
  enum{
       eLoad             = 0, // ������� null-stream
       eCountDown        = 1, // ������� countdown-stream
       eFight            = 2, // ������� ���� �� ���
  };

  enum{
    eTimeCountDown = 30000,// ��
  };

  unsigned short mID_map;

  //struct TStateObject
  //{
  //  unsigned short id;
  //  unsigned int maskState;
  //};
  unsigned char score0;
  unsigned char score1;
  //---------------------------------------------------
  struct TAction
  {
    TTank* pTank;
    nsServerStruct::TPacketServer* pDefPacket;
  };
  //---------------------------------------------------
  // ������ �������:
  // �������� � ����� ������
  //std::list<TStateObject> mListHistoryAction;// �� �������� ������ ���������������� �������� � �������� ��������������� ������
  std::list<TAction*> mListFreshAction;  // ������ ������� �� ��������, ������� ���������

public:
  TRoom();
  ~TRoom();

  void AddTank(TTank* pTank);

	void SetTransport(TTransportProtocol* pTransport);//����� ���, �������� ����������� ������ �� �������

	bool WorkAsRoom(); // ���������� ���������� � ��������� ����� ��������

  void SetIDMap(unsigned short val);
  unsigned short GetIDMap(){return mID_map;};

  TTank* GetTank(int i);

  void MakeGroup();
  void PreparePrediction();// ��������� ������������� ��� ��������� ��������� ��������, ��������, �������
  void LoadMap();

  void SetPacket(nsServerStruct::TPacketServer* pPacket,TTank* ptank);

  void Done();

  // �������
  int GetTimeRest_sec();
  int GetActiveClient();

  virtual void Work(){}

protected:

  virtual float GetTimeWork(){return 0.0f;}

  void WriteTransportStream(TClient* pClient,TBasePacket *packet);
  void WriteTransportAnswer(TClient* pClient,TBasePacket *packet);

  void WorkByState();
  void WorkLoadMap();
  void WorkCountDown();
  void WorkFight();

  bool CheckEndFight();
  
  // �������� ��������� ������ �������� �����
  bool CheckIsLoadComplete();

  void AnalizPacket();// ����� �������� �������������� ����� (����� ��������)
  void CalcPrediction();

  friend void* ThreadLoadMap(void* p);
  void ThreadLoadMap();

  void SendInitCoordTank(TClient* pClient);
  void SendScore(TClient* pClient);
  void SendStateObject(TClient* pClient);

  void InitPositionTank();


};

#endif