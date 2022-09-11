/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
Гудаков Рамиль Сергеевич 
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
class IGraphicEngine; // отрисовка сцены

class IDeveloperTool
{
  NO_COPY_OBJECT(IDeveloperTool)

  bool flgExit;
protected:
	IMakerObjectCommon* mMakerObjectCommon;

	typedef TLogger* (*TInitLogFunc)(); 
	TInitLogFunc mFuncGetLogger;

public:
  struct TComponent
  {
    IControlCamera* mControlCamera; // Camera
    IGUI*           mGUI;           // GUI, MyGUI!
    IGraphicEngine* mGraphicEngine; // BigJack отрисовка сцены
    TComponent()
    {
      mControlCamera = NULL; // Camera
      mGraphicEngine = NULL; // BigJack отрисовка сцены
      mGUI           = NULL; // MyGUI
    }
  };
public:
  IDeveloperTool(){flgExit=false;}
  virtual ~IDeveloperTool(){};
  virtual IMakerObjectCommon* GetMakerObjectCommon() = 0;
  virtual std::string GetPathXMLFile() = 0;

  virtual void Done() = 0;
  virtual void Event(nsEvent::TEvent* pEvent) = 0;

	void Exit(){flgExit = true;}// закончить работу с движком
	bool NeedExit(){return flgExit;}
  void SetInitLogFunc(TInitLogFunc pFunc){mFuncGetLogger=pFunc;}
};

#endif
