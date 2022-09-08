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
  // ��������� ��������� ��� ������������ �������
  void* mErrSource;
  
  // ���� ������������� ���������� ������ ������
  volatile bool mDoDone;
  
  // ���� ������ ������ (���������� ������� ������� ������)
  volatile bool mIsRunned;

protected:  // ������, ���������������� 

  // ���������� ������-��������� ������ � ������.
  // ����� Start() ������ ���������� ������ ����� ���������� 
  // Prepare() � ��������� ���������� ������.
  virtual bool DoPrepare() { return true; }

  // ������� ������� ������. 
  // � �� ����������� ����������� � �����.
  virtual void DoWork()     {}

  // ������������ ���������� ������
  virtual void DoStop() { mDoDone = true; }

  // ���������� ������� ���������� ������ ������
  // ��� ������� ������ ��� �����������
  virtual void OnDestroy() {}
public:
  
  // name - ���������������� �������� ������ ��� ������-���������
  TBL_Thread( const char* name, GThreadPriority priority = G_THREAD_PRIORITY_NORMAL );
  virtual ~TBL_Thread(void);
  
  // ��������� ����� �� ����������
  bool Start();
  
  // ��������� ������ ������
  void Stop();

  // ���� ������ ������ (���������� ������� ������� ������)
  bool IsActive() { return mIsRunned; }  

  static guint CurThreadId();
};
//-----------------------------------------------------------------------------

#endif //GThreadH

