/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ScenarioLoginClient.h"
#include "HiTimer.h"
#include "ManagerSession.h"
#include "Logger.h"
#include "Base.h"
#include "Events.h"
#include "ErrorCode.h"
#include "BL_Debug.h"

using namespace nsMelissa;
using namespace nsLoginClientStruct;
using namespace std;

TScenarioLoginClient::TScenarioLoginClient():
mClient(this),mSlave(this),mMaster(this),mSS(this)
{
  mCurBehavior = NULL;
}
//--------------------------------------------------------------
TScenarioLoginClient::~TScenarioLoginClient()
{

}
//--------------------------------------------------------------
void TScenarioLoginClient::Work()
{
  if(mCurBehavior)
  {
    unsigned int time_ms = ht_GetMSCount();
    mCurBehavior->Work(time_ms);
  }
}
//--------------------------------------------------------------
void TScenarioLoginClient::TryLogin(unsigned int ip, unsigned short port, 
                                   void* data, int size)
{
  mClient.TryLogin(ip, port, data, size);
}
//--------------------------------------------------------------
void TScenarioLoginClient::Recv(TDescRecvSession* pDesc)
{
  mCurBehavior->Recv(pDesc);
}
//--------------------------------------------------------------
void TScenarioLoginClient::Reject(void* resForClient, int sizeResClient)
{
  mMaster.Reject(resForClient, sizeResClient);
}
//--------------------------------------------------------------
void TScenarioLoginClient::Accept(unsigned int key, void* resForClient, int sizeResClient, 
                                  unsigned int id_session_slave)
{
  mMaster.Accept(key, resForClient, sizeResClient, id_session_slave);
}
//--------------------------------------------------------------
void TScenarioLoginClient::Queue(int num)
{
  mMaster.Queue(num);
}
//--------------------------------------------------------------
void TScenarioLoginClient::SetBehavior(eBehavior v)
{
  switch(v)
  {
    case eClient:
      mCurBehavior = &mClient;
      break;
    case eSlave:
      mCurBehavior = &mSlave;
      break;
    case eMaster:
      mCurBehavior = &mMaster;
      break;
    case eSuperServer:
      mCurBehavior = &mSS;
      break;
  }
}
//---------------------------------------------------------------------
void TScenarioLoginClient::SetResultLoginClient(bool isExist)
{
  mSS.SetResultLoginClient(isExist);
}
//---------------------------------------------------------------------