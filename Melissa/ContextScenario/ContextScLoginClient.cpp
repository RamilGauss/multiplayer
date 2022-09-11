/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ContextScLoginClient.h"
#include "BL_Debug.h"

using namespace nsMelissa;

TContextScLoginClient::TContextScLoginClient()
{
  mTimeWaitAnswer = 0;
  mState = eUndef;
}
//------------------------------------------------------------------
TContextScLoginClient::~TContextScLoginClient()
{

}
//------------------------------------------------------------------
void TContextScLoginClient::SetTimeWait(unsigned int v)
{
  mTimeWaitAnswer = v;
}
//------------------------------------------------------------------
unsigned int TContextScLoginClient::GetTimeWait()
{
  return mTimeWaitAnswer;
}
//------------------------------------------------------------------
void TContextScLoginClient::Undef()
{
  mState = eUndef;
}
//------------------------------------------------------------------
void TContextScLoginClient::Accept()
{
  mState = eAccept;
}
//------------------------------------------------------------------
void TContextScLoginClient::Reject()
{
  mState = eReject;
}
//------------------------------------------------------------------
bool TContextScLoginClient::IsReject()
{
  BL_ASSERT(mState!=eUndef);
  return (mState==eReject);
}
//--------------------------------------------------------------
