/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef ManagerObjectCommonH
#define ManagerObjectCommonH

#include "LoaderObjectCommon.h"
#include "IManagerObjectCommon.h"

class IBaseObjectCommon;

class TManagerObjectCommon : public IManagerObjectCommon
{
protected:
  enum{
    eLoadMapEnd=0,
  };

  unsigned int mID_map;

  volatile unsigned char mProcentLoadMap;
  
  volatile bool flgActiveLoadThread;// ���������� ������
  volatile bool flgLoadMap;         // ������� �������� ����
  volatile bool flgNeedStopThreadLoadMap;    // ���������� ���������� ��������

protected:
  
  TLoaderMap mLoaderObject;

public:

  TManagerObjectCommon();
  virtual ~TManagerObjectCommon();

  virtual void Init(IMakerObjectCommon* pMakerObjectCommon);
  virtual void Done();
  virtual void Clear();
  virtual void LoadMap(unsigned int id_map);
  virtual int  GetProgressLoadMap();
  virtual IBaseObjectCommon* CreateObject(unsigned int id_model);

protected:

  //friend void* Thread(void*p);

  //void ThreadLoadMap();
  //void NotifyLoadMapEndEvent();

  // ��������� �����
  void EndLoadMap();// ����� ����� � �������������, ��� ���� ����� �����! �� ���� ������ ���.
  void StopLoadMap();// ���������, �������� ��������� ��������

};

#endif
