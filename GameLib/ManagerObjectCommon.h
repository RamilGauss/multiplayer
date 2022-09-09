/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#ifndef ManagerObjectCommonH
#define ManagerObjectCommonH

#include "LoaderObjectCommon.h"
#include "glib/gthread.h"
#include "IManagerObjectCommon.h"

class IBaseObjectCommon;

class TManagerObjectCommon : public IManagerObjectCommon
{
protected:
  GThread* thread;

  enum{
    eLoadMapEnd=0,
  };

  unsigned int mID_map;

  volatile unsigned char mProcentLoadMap;
  
  volatile bool flgActiveLoadThread;// активность потока
  volatile bool flgLoadMap;         // процесс загрузки идет
  volatile bool flgNeedStopThreadLoadMap;    // необходимо остановить загрузку

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

  // загрузить карту
  void EndLoadMap();// очень криво и отвратительно, что есть такой метод! но пока только так.
  void StopLoadMap();// синхронно, придетс€ подождать маленько

};

#endif
