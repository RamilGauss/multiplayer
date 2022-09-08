/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef BL_ThreadH
#define BL_ThreadH

#include "TypeDef.h"
#include <glib/gthread.h>

//-----------------------------------------------------------------------------
class GBASELIB_EI TBL_Thread
{
  GThread* mThread;
  GThreadPriority mPriority;

  friend gpointer TBL_Thread_threadFunc( gpointer );
  volatile bool mIsPrepared;
  volatile bool mIsSuccessfulStarted;
  void Engine();
protected:
  // Описатель источника для регистратора отказов
  void* mErrSource;
  
  // Флаг необходимости завершения работы потока
  volatile bool mDoDone;
  
  // Флаг работы потока (выполнения рабочей функции потока)
  volatile bool mIsRunned;

protected:  // Методы, переопределяемые 

  // Подготовка модуля-владельца потока к работе.
  // Метод Start() вернет управление только после выполнения 
  // Prepare() в контексте созданного потока.
  virtual bool DoPrepare() { return true; }

  // Рабочая функция потока. 
  // С ее завершением завершается и поток.
  virtual void DoWork()     {}

  // Инициировать завершение потока
  virtual void DoStop() { mDoDone = true; }

  // Обработчик события завершение работы потока
  // Все ресурсы потока уже освобождены
  virtual void OnDestroy() {}
public:
  
  // name - пользовательское название потока или модуля-владельца
  TBL_Thread( const char* name, GThreadPriority priority = G_THREAD_PRIORITY_NORMAL );
  virtual ~TBL_Thread(void);
  
  // Запустить поток на выполнение
  bool Start();
  
  // Завершить работу потока
  void Stop();

  // Флаг работы потока (выполнения рабочей функции потока)
  bool IsActive() { return mIsRunned; }  

  static guint CurThreadId();
};
//-----------------------------------------------------------------------------

#endif //GThreadH

