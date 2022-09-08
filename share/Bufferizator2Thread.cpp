#include "Bufferizator2Thread.h"
#include "TransportProtocolPacket.h"
#include "ApplicationProtocolDef.h"
#include "ClientTank.h"

TBufferizator2Thread GlobalBufferizator2Thread;

void CallBackPacketInterpretator(void* data, int size)
{
  //--------------------------------------------------------
  unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));
  switch(type)
  {
    // фильтр пакетов
    case APPL_TYPE_A_CORRECT_PACKET_STATE_OBJECT:
    case APPL_TYPE_A_CORRECT_PACKET_STATE_TANK:
    case APPL_TYPE_A_SCORE:
    case APPL_TYPE_A_EVENT_IN_FIGHT:
    {
      char* dataPacket = (char*)data + sizeof(TIP_Port);
      int sizePacket   = size - sizeof(TIP_Port);
      GlobalBufferizator2Thread.RcvPacket(dataPacket,sizePacket);
      break;
    }
    default:;
  }
  //--------------------------------------------------------
}
//-------------------------------------------------------------------------------------------
void CallBackStreamInterpretator(void* data, int size)
{
  //--------------------------------------------------------
  unsigned short type = *((unsigned short*)((char*)data+sizeof(TIP_Port)));
  switch(type)
  {
    // фильтр пакетов
    case APPL_TYPE_S_LOAD_MAP:
      break;
    case APPL_TYPE_S_COUNT_DOWN:
    case APPL_TYPE_S_FIGHT_COORD_BULLET:
    {
      char* dataPacket = (char*)data + sizeof(TIP_Port);
      int sizePacket   = size - sizeof(TIP_Port);
      GlobalBufferizator2Thread.RcvStream(dataPacket,sizePacket);
      break;
    }
    default:;
  }
  //--------------------------------------------------------
}
//-------------------------------------------------------------------------------------------

TBufferizator2Thread::TBufferizator2Thread()
:
  mQueuePacket(eCntElemPacket,eSizeElemPacket),
  mQueueStream(eCntElemStream,eSizeElemStream)
{

}
//------------------------------------------------------------------------
TBufferizator2Thread::~TBufferizator2Thread()
{

}
//------------------------------------------------------------------------
void TBufferizator2Thread::RcvPacket(void* dataPacket, int sizePacket)
{
  mQueuePacket.Push(dataPacket,sizePacket);
}
//------------------------------------------------------------------------
void TBufferizator2Thread::RcvStream(void* dataPacket, int sizePacket)
{
  mQueueStream.Push(dataPacket,sizePacket);
}
//------------------------------------------------------------------------
bool TBufferizator2Thread::GetPacket(void* dataPacket, int& sizePacket,guint32 &time_ms)
{
  return mQueuePacket.Pop(dataPacket,sizePacket,time_ms);
}
//------------------------------------------------------------------------
bool TBufferizator2Thread::GetStream(void* dataPacket, int& sizePacket,guint32 &time_ms)
{
  return mQueueStream.PopFresh(dataPacket,sizePacket,time_ms);
}
//------------------------------------------------------------------------
void TBufferizator2Thread::RegisterToClientTank()
{
  GlobalClientTank.Register(CallBackPacketInterpretator,nsCallBackType::eRcvPacket);
  GlobalClientTank.Register(CallBackStreamInterpretator,nsCallBackType::eRcvStream);
}
//------------------------------------------------------------------------
void TBufferizator2Thread::UnregisterFromClientTank()
{
  GlobalClientTank.Unregister(CallBackPacketInterpretator,nsCallBackType::eRcvPacket);
  GlobalClientTank.Unregister(CallBackStreamInterpretator,nsCallBackType::eRcvStream);
}
//------------------------------------------------------------------------
