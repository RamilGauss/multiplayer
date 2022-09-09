#ifndef ComPortH
#define ComPortH

#include "GCS.h"
// Объект серийный интерфейс С2 (RS232, RS242, RS485...)
#if  TD_TARGET == TD_LINUX
  #include <signal.h>
  #include <pthread.h>
#endif
//#include "OutDisk.h"
#include "BL_Log.h"

class TComPort
{
  // .........................................
  // Логирование входного и выходного потоков
  // .........................................
public:
  //static bool mIsPrintLog;
protected:
  TBL_Log mLog;
  //outDisk log;
  void PrintLog( const void* buffer, unsigned long size, bool outStream );

  // .........................................
  // Работа в асинхронном режиме
  // .........................................
protected:
  int	        eventAlert;   // идентификатор "тревожного" события
#if  TD_TARGET == TD_LINUX
  bool        hasThreadID;
  pthread_t   threadID;
  sigset_t    origMask;
#else
  void*       olRead;     // OVERLAPPED*
  void*       olWrite;    // OVERLAPPED*
  int	        eventRead;  // идентификатор события OVERLAPPED
  int	        eventWrite; // идентификатор события OVERLAPPED

	void				checkError();
#endif
protected:
//  int     errCount;   // кол-во зарегистрированных отказов
  bool    autoErrSource;
  void*   errSource;
  void    errRegister( const char *file, int line, guint32 errCode = 0 );
  void    errRegisterLog( const char *file, int line, guint32 errCode = 0 );
protected:
  // Общее кол-во отправленных/прочитанных байт
  unsigned long cntWrited, cntReaded;
  // Дескриптор устройства
  int	    handle;
  // Номер порта
  int     portNumber;
  // ожидание чтения, по умолчанию
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
  // Вкл/выкл контроль многопоточного доступа.
  // По умолчанию - выключен. Для использования setAlert() - включать обязательно
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

  // Читать данные с порта пока не выполниться любое из условий:
  // 1) поступили данные в приемный буфер buf[size]
  // 2) не наступил таймаут <timeout> мс
  // 3) не произошло "тревожном" событие <alert()>
  // Параметры:
  // buf - приемный буфер
  // size [IN] - размер приемного буфера
  //      [OUT]- принятый объем данных
  // timeout - величина таймаута, мс. Если = -1, то таймтаут по
  //           умолчанию (см. метод setPortState() ).
  // Результат: см. rr_XXX
  virtual int read( void* buf, unsigned long& size, long timeout = -1 );

  enum {
    rr_Error,   // возникновение отказа
    rr_Read,    // поступили даные
    rr_Timeout, // наступил таймаут
    rr_Alert    // наступило тревожное событие
  };

  // Уведомить о "тревожном" событии ("разбудить" операцию чтения)
  void  setAlert();

  virtual bool	isDSR();
  virtual bool  isCTS();

  virtual bool  setRTS(bool);
  virtual bool  setDTR(bool);

  virtual bool setPortState
        (
        unsigned long baudRate, // скорость, бит в секунду
        unsigned char byteSize, // кол-во информационных бит в посылке( 8 или 7 )
        unsigned char parity,   // 0 - Нет проверки четности, 1 - Проверка четности, 2 - Проверка нечетности
        unsigned char stopBits, // кол-во стоповых бит: 0 - 1 бит, 1 - 2 бита
        unsigned char isCTR_RTS,// вкл. аппаратное управление потоком
        long waitRead_ms = -1   // ожидание чтения, по умолчанию
        //0: - без ожидания
        //-1 - без таймаута
        //иначе - время ожидания, мс
        //Может переопределятся в функции read() для конкретной операции
        );

  // Очистить очереди порта
  // input -  входную очередь
  // output - выходную очередь
  //void clearFIFO( bool input, bool output );

  // Очистить входную очередь порта
  virtual void clearInputQueue(void);

  enum { sizeListSpeed = 20 };
  static unsigned long listSpeed[sizeListSpeed];

  static inline unsigned long getSpeed( int index )
    {  return ( index < 0 || index > sizeListSpeed ) ? 1200 : listSpeed[index]; }

};

#endif

