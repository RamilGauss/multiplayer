/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 

#include <map>
#include <string>

#include "Only_N_Object.h"
#include "GCS.h"


typedef std::map<std::string,int> TMapStrInt;
typedef std::map<void*,std::string> TMapPtrStr;
//----------------------------------------------------------------------------
namespace nsStoreObject
{
  enum{ePush=0,ePop=1,};
}
namespace nsLockThread
{
  enum{eLock=0,eUnlock=1,};
}
//----------------------------------------------------------------------------
void LockThread(int lock);
void StoreObject(int i, int maxCntObject, std::string nameClass, void* pThis);
//----------------------------------------------------------------------------
TOnly_N_Object::TOnly_N_Object(int maxCntObject, std::string nameClass)
{
  StoreObject(nsStoreObject::ePush,maxCntObject,nameClass,this);
}
//----------------------------------------------------------------------------
TOnly_N_Object::~TOnly_N_Object()
{
  StoreObject(nsStoreObject::ePop,0,"",this);
}
//----------------------------------------------------------------------------
// like C-style
void StoreObject(int i, int maxCntObject, std::string nameClass, void* pThis)
{
  static TMapStrInt mMapClassCnt;// для нахождения по имени класса макс. кол-ва объектов
  static TMapPtrStr mMapPtrName; //                по указателю на объект имя класса
  int cntMake = 0;
  bool res = false;

  LockThread(nsLockThread::eLock);// блокировка
  switch(i)
  {
    case nsStoreObject::ePush:
    {
      //-------------------------------
      TMapStrInt::iterator fit = mMapClassCnt.find(nameClass);
      if(fit==mMapClassCnt.end())
      {
        // не нашли - добавить
        mMapClassCnt.insert(TMapStrInt::value_type(nameClass,1));
        fit = mMapClassCnt.find(nameClass);

        // что бы потом найти в деструкторе
        mMapPtrName.insert(TMapPtrStr::value_type(pThis,nameClass));
      }
      else
        fit->second++;

      cntMake = fit->second;
    }
    break;
    case nsStoreObject::ePop:
    {
      //-------------------------------
      TMapPtrStr::iterator fit_name = mMapPtrName.find(pThis);
      if(fit_name!=mMapPtrName.end())
      {
        TMapStrInt::iterator fit = mMapClassCnt.find(fit_name->second);
        if(fit!=mMapClassCnt.end())
        {
          fit->second--;
          cntMake = fit->second;
          res = true;
        }
      }
    }
    break;
  }
  LockThread(nsLockThread::eUnlock);// разблокировать
  // проверка результатов
  switch(i)
  {
    case nsStoreObject::ePush:
      BL_ASSERT(cntMake <= maxCntObject);
      break;
    case nsStoreObject::ePop:
      BL_ASSERT(cntMake >= 0 && res);
      break;
  }
}
//----------------------------------------------------------------------------
void LockThread(int lock)
{
  static GCS gcs;
  switch(lock)
  {
    case nsLockThread::eLock:
      gcs.lock();
      break;
    case nsLockThread::eUnlock:
      gcs.unlock();
      break;
  }
}
//----------------------------------------------------------------------------
