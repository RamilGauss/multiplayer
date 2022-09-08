#include "Prediction.h"
#include "ApplicationProtocolPacketCmd.h"

using namespace nsServerStruct;

TPrediction::TPrediction()
{

}
//--------------------------------------------------------------------
TPrediction::~TPrediction()
{

}
//--------------------------------------------------------------------
void TPrediction::InitState()
{

}
//--------------------------------------------------------------------
void TPrediction::Calc()
{

}
//--------------------------------------------------------------------
void TPrediction::SetOrientAim(TTank* pTank, TPacketServer* pDefPacket)
{
  
}
//--------------------------------------------------------------------
void TPrediction::SetKeyEvent(TTank*pTank, TPacketServer* pDefPacket)
{
  TC_Key_Event* packet = (TC_Key_Event*)pDefPacket->packet;
  pTank->mMaskPushButton &= packet->getKeyEvent();
  pTank->mTimeRefreshPushButton = pDefPacket->ms_time;
}
//--------------------------------------------------------------------
