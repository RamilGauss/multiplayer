#ifndef Bufferizator2ThreadH
#define Bufferizator2ThreadH

#include "MultiThreadQueue.h"

// настраивают снаружи для обработки пакетов и стрима от транспорта

class TBufferizator2Thread
{
  bool flgWasRegisterCallback;

  enum{
       eCntElemStream   = 200,
       eSizeElemStream  = 4000,
       eCntElemPacket   = 50,
       eSizeElemPacket  = 4000,
  };

  TMultiThreadQueue mQueuePacket; // корректирующий пакет
  TMultiThreadQueue mQueueStream; // мгновенный слепок с координат танков и снарядов, 
  // если танк невидим, то его в стриме не будет

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
