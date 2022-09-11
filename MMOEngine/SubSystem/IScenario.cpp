/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "IScenario.h"
#include <stddef.h>
#include "BL_Debug.h"
#include "Base.h"
#include "IContextScenario.h"

using namespace nsMMOEngine;

IScenario::IScenario()
{
  mCurContext = NULL;
}
//---------------------------------------------------------------------
IScenario::~IScenario()
{

}
//---------------------------------------------------------------------
bool IScenario::Begin()
{
  return mCurContext->Activate();
}
//---------------------------------------------------------------------
void IScenario::End()
{
	// уведомить об окончании сценария
	mCallBackEnd.Notify(this);
	// сценарий закончен
  mCurContext->Disactivate();
}
//---------------------------------------------------------------------
void IScenario::DelayBegin()
{
  BL_FIX_BUG();
}
//---------------------------------------------------------------------
void IScenario::Work()
{

}
//---------------------------------------------------------------------
void IScenario::SetContext(IContextScenario* pCSc)
{
  mCurContext = pCSc;
}
//---------------------------------------------------------------------
IContextScenario* IScenario::GetContext()
{
  return mCurContext;
}
//---------------------------------------------------------------------
void IScenario::NeedContext(unsigned int id_session)
{
  mCallBackNeedContext.Notify(id_session);
}
//---------------------------------------------------------------------
unsigned char IScenario::GetType()
{
  return mType;
}
//---------------------------------------------------------------------
void IScenario::SetType(unsigned char type)
{
  mType = type;
}
//---------------------------------------------------------------------