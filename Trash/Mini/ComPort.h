#ifndef ComPortH
#define ComPortH

#include "GCS.h"
// ������ �������� ��������� �2 (RS232, RS242, RS485...)
#if  TD_TARGET == TD_LINUX
  #include <signal.h>
  #include <pthread.h>
#endif
//#include "OutDisk.h"
#include "BL_Log.h"

class TComPort
{
  // .........................................
  // ����������� �������� � ��������� �������
  // .........................................
public:
  //static bool mIsPrintLog;
protected:
  TBL_Log mLog;
  //outDisk log;
  void PrintLog( const void* buffer, unsigned long size, bool outStream );

  // .........................................
  // ������ � ����������� ������
  // .........................................
protected:
  int	        eventAlert;   // ������������� "����������" �������
#if  TD_TARGET == TD_LINUX
  bool        hasThreadID;
  pthread_t   threadID;
  sigset_t    origMask;
#else
  void*       olRead;     // OVERLAPPED*
  void*       olWrite;    // OVERLAPPED*
  int	        eventRead;  // ������������� ������� OVERLAPPED
  int	        eventWrite; // ������������� ������� OVERLAPPED

	void				checkError();
#endif
protected:
//  int     errCount;   // ���-�� ������������������ �������
  bool    autoErrSource;
  void*   errSource;
  void    errRegister( const char *file, int line, guint32 errCode = 0 );
  void    errRegisterLog( const char *file, int line, guint32 errCode = 0 );
protected:
  // ����� ���-�� ������������/����������� ����
  unsigned long cntWrited, cntReaded;
  // ���������� ����������
  int	    handle;
  // ����� �����
  int     portNumber;
  // �������� ������, �� ���������
  long default_timeout_ms;

  bool  isFlag( int flag );
#if  TD_TARGET == TD_LINUX
  bool  setFlag( bool set, int flag );
#else
  bool  setFlag( int flag );
#endif
protected:
  GCS  cs;
  void lock()   { cs.lock(); }
  void unlock() { cs.unlock(); }
public:
  // ���/���� �������� �������������� �������.
  // �� ��������� - ��������. ��� ������������� setAlert() - �������� �����������
  TComPort();// bool _mt_control = false );
  virtual ~TComPort();

  void  setErrSource( void* es ) { errSource = es; autoErrSource = false; }
  void  errRegisterStr( const char *msg, const char *file, int line );
  void*   ErrSource();

  virtual bool  open( int pn );
  bool  reopen()
  {
    close();
    return open( portNumber );
  }
  virtual void  close();

  virtual bool  isOpen();
  int   NumberPort() { return portNumber; }
  int   Handle()     { return handle; }

  virtual bool  write( const void* lpvoid, unsigned long sz );
  bool  write( const char value ) { return write( &value, 1UL ); }

  // ������ ������ � ����� ���� �� ����������� ����� �� �������:
  // 1) ��������� ������ � �������� ����� buf[size]
  // 2) �� �������� ������� <timeout> ��
  // 3) �� ��������� "���������" ������� <alert()>
  // ���������:
  // buf - �������� �����
  // size [IN] - ������ ��������� ������
  //      [OUT]- �������� ����� ������
  // timeout - �������� ��������, ��. ���� = -1, �� �������� ��
  //           ��������� (��. ����� setPortState() ).
  // ���������: ��. rr_XXX
  virtual int read( void* buf, unsigned long& size, long timeout = -1 );

  enum {
    rr_Error,   // ������������� ������
    rr_Read,    // ��������� �����
    rr_Timeout, // �������� �������
    rr_Alert    // ��������� ��������� �������
  };

  // ��������� � "���������" ������� ("���������" �������� ������)
  void  setAlert();

  virtual bool	isDSR();
  virtual bool  isCTS();

  virtual bool  setRTS(bool);
  virtual bool  setDTR(bool);

  virtual bool setPortState
        (
        unsigned long baudRate, // ��������, ��� � �������
        unsigned char byteSize, // ���-�� �������������� ��� � �������( 8 ��� 7 )
        unsigned char parity,   // 0 - ��� �������� ��������, 1 - �������� ��������, 2 - �������� ����������
        unsigned char stopBits, // ���-�� �������� ���: 0 - 1 ���, 1 - 2 ����
        unsigned char isCTR_RTS,// ���. ���������� ���������� �������
        long waitRead_ms = -1   // �������� ������, �� ���������
        //0: - ��� ��������
        //-1 - ��� ��������
        //����� - ����� ��������, ��
        //����� ��������������� � ������� read() ��� ���������� ��������
        );

  // �������� ������� �����
  // input -  ������� �������
  // output - �������� �������
  //void clearFIFO( bool input, bool output );

  // �������� ������� ������� �����
  virtual void clearInputQueue(void);

  enum { sizeListSpeed = 20 };
  static unsigned long listSpeed[sizeListSpeed];

  static inline unsigned long getSpeed( int index )
    {  return ( index < 0 || index > sizeListSpeed ) ? 1200 : listSpeed[index]; }

};

#endif

