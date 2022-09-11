/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include "ManagerObjectCommon.h"
#include <stddef.h>
#include "Logger.h"
#include "HiTimer.h"

//-----------------------------------------------------------------------------
TManagerObjectCommon::TManagerObjectCommon()
{
  flgLoadMap               = false;
  flgActiveLoadThread      = false;
}
//--------------------------------------------------------------------
TManagerObjectCommon::~TManagerObjectCommon()
{
  Done();
}
//--------------------------------------------------------------------
//void* Thread(void* p)
//{
  //((TManagerObjectCommon*)p)->ThreadLoadMap();
  //return NULL;
//}
//--------------------------------------------------------------------
//void TManagerObjectCommon::ThreadLoadMap()
//{
  /*GlobalLoggerMOC.WriteF_time("�������� �������� �����.\n");
  flgActiveLoadThread = true;

  mProcentLoadMap = 0;
  CHECK_END
  mLoaderObject.LoadMap(mID_map);
  mProcentLoadMap = 15;
  CHECK_END
  AddFromLoaderObjectInCommonList();
  mProcentLoadMap = 55;
  CHECK_END
  AddFromLoaderObjectInMDX();
  mProcentLoadMap = 70;
  CHECK_END
  AddFromLoaderObjectInPrediction();
  mProcentLoadMap = 80;

  CHECK_END
  //---------------------------------------
  NotifyLoadMapEndEvent();
  //---------------------------------------
  flgActiveLoadThread = false;
  
  GlobalLoggerMOC.WriteF_time("�������� ����� ��������� (����� MOC).\n");
  GlobalLoggerMOC.WriteF_time("�������� ��������������� ������.\n");*/
//}
//--------------------------------------------------------------------
void TManagerObjectCommon::LoadMap(unsigned int id_map)
{
  //flgNeedStopThreadLoadMap = false;
  //flgLoadMap               = true;
  //mID_map = id_map;
  //thread = g_thread_create(Thread, this, true, NULL);
}
//--------------------------------------------------------------------
int TManagerObjectCommon::GetProgressLoadMap()
{
  return 100;
}
//--------------------------------------------------------------------
void TManagerObjectCommon::Done()
{
  Clear();
}
//--------------------------------------------------------------------
void TManagerObjectCommon::EndLoadMap()
{
  GetLogger()->Get("MOC")->WriteF_time("�������� ����� ��������� (����� Form).\n");
  mProcentLoadMap          = 100;
  flgLoadMap               = false;
}
//--------------------------------------------------------------
void TManagerObjectCommon::StopLoadMap()// ���������, �������� ��������� ��������
{
  GetLogger()->Get("MOC")->WriteF_time("������ ���������� �������� �����.\n");
  flgNeedStopThreadLoadMap = true;
}
//-------------------------------------------------------------------------------------------------------------  
void TManagerObjectCommon::Init(IMakerObjectCommon* pMakerObjectCommon)
{
  mLoaderObject.SetMakerObjectCommon(pMakerObjectCommon);
}
//--------------------------------------------------------------------------------------------------------
void TManagerObjectCommon::Clear()
{
  int cnt = mVectorObject.size();
  for(int i = 0 ; i < cnt ; i++ )
    mLoaderObject.Delete(mVectorObject[i]);

  mVectorObject.clear();
}
//--------------------------------------------------------------------------------------------------------
IBaseObjectCommon* TManagerObjectCommon::CreateObject(unsigned int id_model)
{
  IBaseObjectCommon* pObject = mLoaderObject.LoadObject(id_model);
  AddObject(pObject);
  return pObject;
}
//--------------------------------------------------------------------------------------------------------
