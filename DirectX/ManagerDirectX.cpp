#include "ManagerDirectX.h"
#include "HiTimer.h"
#include "Bufferizator2Thread.h"
#include "ClientTank.h"
#include "LogerDX.h"


TManagerDirectX GlobalManagerDirectX;

TManagerDirectX::TManagerDirectX()
{

}
//--------------------------------------------------------------------------------------------------------
TManagerDirectX::~TManagerDirectX()
{

}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Work()
{
  Calc();
  Optimize();
  Render();
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Refresh()
{
  // только когда загрузится карта и перед тем как отошлется запрос на корректирующий пакет.
  if(flgNeedSendCorrectPacket)
  {
    flgNeedSendCorrectPacket = false;
    GlobalBufferizator2Thread.RegisterToClientTank();
    GlobalClientTank.SendRequestCorrectPacket();

    GlobalLoggerDX.WriteF_time("Отослан корректирующий пакет.\n");
  }

  char bufferStream[4000];
  char bufferPacket[4000];
  int sizeStream = sizeof(bufferStream);
  int sizePacket = sizeof(bufferPacket);
  guint32 time_ms_packet;
  guint32 time_ms_stream;
  bool getPacket = GlobalBufferizator2Thread.GetPacket(bufferPacket,sizePacket,time_ms_packet);
  bool getStream = GlobalBufferizator2Thread.GetStream(bufferStream,sizeStream,time_ms_stream);
  
  //if(getPacket)
  //  GlobalLoggerDX.WriteF_time("Получен пакет.\n");
  //if(getStream)
  //  GlobalLoggerDX.WriteF_time("Получен стрим.\n");
  //else
  //  GlobalLoggerDX.WriteF_time("-----------------------------------------------\n");
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Calc()
{

}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Optimize()
{

}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Render()
{

}
//--------------------------------------------------------------------------------------------------------
void* ThreadLoadMap(void* p)
{
  ((TManagerDirectX*)(p))->ThreadLoadMap();
  return NULL;
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::ThreadLoadMap()
{
  ht_msleep(5000);//### симуляция
  flgLoadingMap = false;
  flgNeedSendCorrectPacket = true;
  
  GlobalLoggerDX.WriteF_time("Загрузка карты завершена.\n");
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::LoadMap(TA_In_Fight& packet)
{
  GlobalLoggerDX.WriteF_time("Загрузка карты началась.\n");

  flgLoadingMap = true;
  mPacket.setData(packet.getData(),packet.getSize());
  threadLoadMap = g_thread_create(::ThreadLoadMap, (gpointer)this, true, NULL);
}
//--------------------------------------------------------------------------------------------------------