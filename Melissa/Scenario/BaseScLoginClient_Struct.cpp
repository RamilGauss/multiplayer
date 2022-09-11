/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "BaseScLoginClient_Struct.h"
#include "MakerScenario.h"

using namespace nsMelissa;
using namespace nsLoginClientStruct;

THeader::THeader()
{
  type      = TMakerScenario::eLoginClient;
  id_client = -1;
}
//---------------------------------------------------------------------------------------
THeaderTryLoginC2M::THeaderTryLoginC2M()
{
  subType  = eTryLoginC2M;
  sizeData = 0;
}
//--------------------------------------------------------------
THeaderRequestM2SS::THeaderRequestM2SS()
{
  subType = eRequestM2SS;
}
//--------------------------------------------------------------
THeaderCheckRequestSS2M::THeaderCheckRequestSS2M()
{
  subType         = eCheckRequestSS2M;
  isExistInSystem = 0;
}
//--------------------------------------------------------------
THeaderResultLoginM2C::THeaderResultLoginM2C()
{
  subType       = eResultLoginM2C;
  numInQueue    = 0;
  sizeResClient = 0;
}
//--------------------------------------------------------------
THeaderLeaveQueueC2M::THeaderLeaveQueueC2M()
{
  subType = eLeaveQueueC2M;
}
//--------------------------------------------------------------
THeaderCheckLeaveM2C::THeaderCheckLeaveM2C()
{
  subType = eCheckLeaveQueueM2C;
}
//------------------------------------------------------------------------------------------------
THeaderInfoClientM2S::THeaderInfoClientM2S()
{
  subType = eInfoClientM2S;
}
//--------------------------------------------------------------
THeaderCheckInfoClientM2S::THeaderCheckInfoClientM2S()
{
  subType = eCheckInfoClientS2M;
}
//--------------------------------------------------------------
THeaderInfoSlaveM2C::THeaderInfoSlaveM2C()
{
  subType = eInfoSlaveM2C;
}
//--------------------------------------------------------------
THeaderCheckInfoSlaveC2M::THeaderCheckInfoSlaveC2M()
{
  subType = eCheckInfoSlaveC2M;
}
//--------------------------------------------------------------
THeaderConnectToSlaveC2S::THeaderConnectToSlaveC2S()
{
  subType = eConnectToSlaveC2S;
}
//--------------------------------------------------------------
THeaderClientConnectS2M::THeaderClientConnectS2M()
{
  subType = eClientConnectS2M;
}
//--------------------------------------------------------------
THeaderCheckClientConnectM2S::THeaderCheckClientConnectM2S()
{
  subType = eCheckClientConnectM2S;
}
//--------------------------------------------------------------
THeaderCheckConnectToSlaveS2C::THeaderCheckConnectToSlaveS2C()
{
  subType = eCheckConnectToSlaveS2C;
}
//--------------------------------------------------------------
