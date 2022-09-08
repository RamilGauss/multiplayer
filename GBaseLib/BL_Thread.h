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

