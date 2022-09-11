/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IDeveloperToolH
#define IDeveloperToolH

#include "classTools.h"
#include <string>
#include "DescEvent.h"
#include "IMakerObjectCommon.h"
#include "Logger.h"

class IControlCamera;
class IGUI;
class IGraphicEngine; // ��������� �����

class IDeveloperTool
{
  NO_COPY_OBJECT(IDeveloperTool)

  bool flgExit;
protected:
	IMakerObjectCommon* mMakerObjectCommon;

	typedef TLogger* (*TInitLogFunc)(); 
	TInitLogFunc mFuncGetLogger;
public:
  IDeveloperTool(){flgExit=false;}
  virtual ~IDeveloperTool(){};
  virtual IMakerObjectCommon* GetMakerObjectCommon() = 0;
  virtual std::string GetPathXMLFile() = 0;

  virtual void Done() = 0;
  virtual void Event(nsEvent::TEvent* pEvent) = 0;

	void Exit(){flgExit = true;}// ��������� ������ � �������
	bool NeedExit(){return flgExit;}
  void SetInitLogFunc(TInitLogFunc pFunc){mFuncGetLogger=pFunc;}
};

#endif
