#ifndef Bufferizator2ThreadH
#define Bufferizator2ThreadH

#include "MultiThreadQueue.h"

// ����������� ������� ��� ��������� ������� � ������ �� ����������

class TBufferizator2Thread
{
  bool flgWasRegisterCallback;

  enum{
       eCntElemStream   = 200,
       eSizeElemStream  = 4000,
       eCntElemPacket   = 50,
       eSizeElemPacket  = 4000,
  };

  TMultiThreadQueue mQueuePacket; // �������������� �����
  TMultiThreadQueue mQueueStream; // ���������� ������ � ��������� ������ � ��������, 
  // ���� ���� �������, �� ��� � ������ �� �����

public:
  TBufferizator2Thread();
  ~TBufferizator2Thread();

  void ClearWasRegisterCallback(){flgWasRegisterCallback=false;}

  void RcvPacket(void* dataPacket, int sizePacket);
  void RcvStream(void* dataPacket, int sizePacket);
  
  bool GetPacket(void* dataPacket, int& sizePacket,guint32 &time_ms);
  bool GetStream(void* dataPacket, int& sizePacket,guint32 &time_ms);


  void RegisterToClientTank();
  void UnregisterFromClientTank();

};

extern TBufferizator2Thread GlobalBufferizator2Thread;

#endif
