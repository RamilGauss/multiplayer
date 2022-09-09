#ifndef _GNU_SOURCE
  #define _GNU_SOURCE
#endif

#include <time.h>
#include "ComPort.h"
#include "BL_Debug.h"
#include "ErrorReg.h"
#include <stdio.h>
#include "BL_AppFile.h"


#define ERR_REG()                 errRegister(  __FILE__, __LINE__ )
#define ERR_REGCODE( code )       errRegister(  __FILE__, __LINE__, code )

#define ERR_REGLOG()              errRegisterLog(   __FILE__, __LINE__ )
#define ERR_REGCODELOG( code )    errRegisterLog(  __FILE__, __LINE__, code )
//------------------------------------------------------------------------------
//==============================================================================
//==============================================================================
//==============================================================================
#ifdef TD_WINDOWS
  #include <windows.h>
#else 
  #include <errno.h>
  #include <string.h>
#endif
//------------------------------------------------------------------------------
void TComPort::errRegisterLog( const char *file, int line, guint32 errCode )
{
  TErrorDef def;
  time( &def.time );
  #if TD_TARGET != TD_LINUX
    def.code = errCode ? errCode : GetLastError();
    def.group = egSDK;
  #else
    def.code = errCode ? errCode : errno;
    def.group = egSTD;
  #endif
  def.file = file;
  def.line = line;
  def.source = ErrSource();
  TErrorDesc desc;
  errDesc_Fill( def, desc );
  mLog.printf( bl_locale, desc.str );
  //BL_Log( desc.str );
}
//==============================================================================
//==============================================================================
//==============================================================================
unsigned long TComPort::listSpeed[] =
{
  50,     75,     110,    134,    150,  200,    300,  600,
  1200,   1800,   2400,   4800,   9600, 19200,  38400,
	57600,  115200, 230400, 460800, 921600
};
#if TD_TARGET != TD_LINUX
//  #include <windows.h>

#define OLREAD    ((OVERLAPPED*)olRead)
#define OLWRITE   ((OVERLAPPED*)olWrite)
#define EVREAD    (*(HANDLE*)&eventRead)
#define EVWRITE   (*(HANDLE*)&eventWrite)
#define EVALERT   (*(HANDLE*)&eventAlert)

//------------------------------------------------------------------------------
TComPort::TComPort( ):cs("TComPort")
{
  errSource = NULL;
  autoErrSource = false;
  portNumber = -1;
  handle = (int)INVALID_HANDLE_VALUE;

  EVALERT = CreateEvent( NULL, TRUE, FALSE, NULL );

  EVREAD = CreateEvent( NULL, TRUE, FALSE, NULL );
  olRead = new OVERLAPPED;
  memset( olRead, 0, sizeof(OVERLAPPED) );
  OLREAD->hEvent = EVREAD;

  EVWRITE = CreateEvent( NULL, TRUE, FALSE, NULL );
  olWrite = new OVERLAPPED;
  memset( olWrite, 0, sizeof(OVERLAPPED) );
  OLWRITE->hEvent = EVWRITE;

  cntWrited = cntReaded = 0;
}
//------------------------------------------------------------------------------
TComPort::~TComPort()
{
  close();
  CloseHandle( EVALERT );
  CloseHandle( EVREAD );
  delete olRead;
  delete olWrite;
}
//------------------------------------------------------------------------------
void TComPort::close()
{
  lock();
  if ( handle != (int)INVALID_HANDLE_VALUE )
  {
    CloseHandle( (HANDLE)handle );
    handle = (int)INVALID_HANDLE_VALUE;
  }
  if( autoErrSource ) errSource = NULL;
  mLog.setName( NULL );
  unlock();
}
//------------------------------------------------------------------------------
//Открытие порта
bool TComPort::open( int pn )
{
  // Закрытие предыдущего порта
  close();

  bool ret = false;
  lock();
  portNumber = pn;
  // Запись номера порта в строке
  char str[20];
  wsprintf( str, "\\\\.\\COM%d", pn );

  // Попытка открытия устройства
  handle = (int)
    CreateFileA(
      str,                              // имя
      GENERIC_READ | GENERIC_WRITE,     // читать и писать
      0,//FILE_SHARE_READ|FILE_SHARE_WRITE,  // не разделять
      NULL,                             // нет Security Attr
      OPEN_EXISTING,                    // считаем, что он есть
      FILE_FLAG_OVERLAPPED,             // ассинхронно
      NULL );                           // прототипа нет
  if ( handle != (int)INVALID_HANDLE_VALUE )
  {
		ResetEvent( EVALERT );
		//errCount = 0;
    ret = true;
  }
  else
    errSDK_SOURCE( ErrSource() );
  unlock();
  return ret;
}
//------------------------------------------------------------------------------
bool TComPort::isOpen()
{
  return handle != (int)INVALID_HANDLE_VALUE;
}
//------------------------------------------------------------------------------
void TComPort::errRegister( const char *file, int line, guint32 errCode )
{
  if( !errCode ) errCode = GetLastError();
  errSDK_Set( errCode, file, line, ErrSource() );
}
//------------------------------------------------------------------------------
bool TComPort::write( const void* lpvoid, unsigned long sz )//, long timeout )
{
  if( !sz ) return true;
  BOOL ret = FALSE;

  lock();
  if ( handle != (int)INVALID_HANDLE_VALUE )
  {
    do
    {
      DWORD bytesWritten;
      PrintLog( lpvoid, sz, true );    //###
      ret = WriteFile( (HANDLE)handle, lpvoid, sz, &bytesWritten, OLWRITE );
      if( !ret )
      {
        DWORD errCode = GetLastError();
        if( errCode != ERROR_IO_PENDING )
        {
          ERR_REGCODELOG( errCode );
          goto l_exit;
        }
        switch( WaitForSingleObject( OLWRITE->hEvent, INFINITE ) )//timeout ) )
        {
          case WAIT_OBJECT_0:
            if( SUCCEEDED( OLWRITE->Internal ) )
            {
              cntWrited += OLWRITE->InternalHigh;
              sz -= OLWRITE->InternalHigh;
              lpvoid = (char*)lpvoid + OLWRITE->InternalHigh;
            }
            else
            {
              ERR_REGCODELOG( OLWRITE->Internal & 0xFFFF );
              goto l_exit;
            }
            break;
          case WAIT_TIMEOUT:
            ERR_REG();
            goto l_exit;
        }
      }
      else
      {
        cntWrited += bytesWritten;
        sz -= bytesWritten;
        lpvoid = (char*)lpvoid + bytesWritten;
      }
    } while( sz != 0 );
    ret = TRUE;
  }
l_exit:
  unlock();

  return ret ? true : false;
}
//------------------------------------------------------------------------------
void TComPort::checkError()
{
  DWORD Errors;
  COMSTAT Stat;
  if( ClearCommError( (HANDLE)handle, &Errors, &Stat ) )
  {
    if( Stat.cbInQue )
      BL_Log( "Stat.cbInQue = %d", Stat.cbInQue );
    if( Errors )
      BL_Log( "Errors = %X", Errors );
  }
  else ERR_REG();
}
//------------------------------------------------------------------------------
int TComPort::read( void* buf, unsigned long& size, long timeout )
{
  DWORD errCode;
  int ret = rr_Error;
  if( timeout == -1 ) timeout = default_timeout_ms;

  lock();
  if ( handle != (int)INVALID_HANDLE_VALUE )
  {
    DWORD bytesRead = size;
    size = 0;
    if( ReadFile( (HANDLE)handle, buf, bytesRead, &size, OLREAD ) )
    {
      PrintLog( buf, size, false ); //###
      ret = rr_Read;
      cntReaded += size;
    }
    else
    {
      errCode = GetLastError();
      if( errCode != ERROR_IO_PENDING )
      {
        ERR_REGCODELOG( errCode );
        CancelIo( (HANDLE)handle );
      }
      else
      {
        HANDLE arrEvent[2];
        arrEvent[0] = EVREAD;
        arrEvent[1] = EVALERT;
        switch( WaitForMultipleObjects( 2, arrEvent, FALSE, timeout ) )
        {
          case WAIT_OBJECT_0+0: // Чтение успешно
            if( SUCCEEDED(OLREAD->Internal)  )
            {
              size = OLREAD->InternalHigh;
              PrintLog( buf, size, false ); //###
              cntReaded += size;
              ret = rr_Read;
            }
            else
						{
              ERR_REGCODELOG( OLREAD->Internal & 0xFFFF );
						}
            break;
          case WAIT_OBJECT_0+1: // "тревожное" событие
            ret = rr_Alert;
            ResetEvent( EVALERT );
            //PurgeComm( (HANDLE)handle, PURGE_RXABORT );
						CancelIo( (HANDLE)handle );
            break;
          case WAIT_TIMEOUT:    // Таймаут
            ret = rr_Timeout;
            //PurgeComm( (HANDLE)handle, PURGE_RXABORT );
						CancelIo( (HANDLE)handle );
            break;
          default:
            ERR_REG();
            //PurgeComm( (HANDLE)handle, PURGE_RXABORT );
						CancelIo( (HANDLE)handle );
        }
      }//-
    }//-
  }//-
  unlock();
  return ret;
}
//------------------------------------------------------------------------------
// Уведомить о "тревожном" событии
void TComPort::setAlert()
{
  SetEvent( EVALERT );
}
//------------------------------------------------------------------------------
bool TComPort::isFlag( int flag )
{
  DWORD status = 0;
  bool res = false;
  lock();
  if ( handle != (int)INVALID_HANDLE_VALUE )
  {
    if( GetCommModemStatus( (HANDLE)handle, &status ) )
    {
      if( status & flag )
        res = true;
    }
    else
    {
      ERR_REG();
      CancelIo( (HANDLE)handle );
    }
  }
  unlock();
  return res;
}
//------------------------------------------------------------------------------
bool TComPort::isDSR()
{
  return isFlag( MS_DSR_ON );
}
//------------------------------------------------------------------------------
bool TComPort::isCTS()
{
  return isFlag( MS_CTS_ON );
}
//------------------------------------------------------------------------------
bool TComPort::setFlag( int flag )
{
  bool res = false;
  lock();
  if ( handle != (int)INVALID_HANDLE_VALUE )
  {
    if ( EscapeCommFunction( (HANDLE)handle, flag ) )
      res = true;
    else
      ERR_REG();
  }
  unlock();
  return res;
}
//------------------------------------------------------------------------------
bool TComPort::setRTS( bool set )
{
  return setFlag( set ? SETRTS : CLRRTS );
}
//------------------------------------------------------------------------------
bool TComPort::setDTR( bool set )
{
  return setFlag( set ? SETDTR : CLRDTR );
}
//------------------------------------------------------------------------------
// Установка состояния порта
// waitRead_ms  - ожидание чтения
//    0: - без ожидания
//    -1 - без таймаута
//    иначе - время ожидания, мс
bool TComPort::setPortState(
  unsigned long baudRate,   // скорость
  unsigned char byteSize,   // кол-во бит
  unsigned char parity,     // четность
  unsigned char stopBits,   // кол-во стоповых бит
  unsigned char isCTR_RTS,  // аппаратное управление потоком
  long waitRead_ms          // ожидание чтения
)
{
  bool res = false;
  lock();
  if( handle != (int)INVALID_HANDLE_VALUE )
  {
    DCB dcb;
    memset( &dcb, 0, sizeof(dcb) );
    dcb.DCBlength  = sizeof( dcb );
    dcb.BaudRate   = baudRate;
    dcb.ByteSize   = byteSize;
    dcb.Parity     = parity;
    dcb.StopBits   = stopBits;
    dcb.fBinary    = 1;
    dcb.fAbortOnError = 1;

    // аппаратное управление потоком
    if( isCTR_RTS )
    {
      dcb.fOutxCtsFlow = TRUE;
      dcb.fRtsControl = RTS_CONTROL_TOGGLE;
    }
    else
      dcb.fRtsControl = RTS_CONTROL_ENABLE;

    if ( SetCommState( (HANDLE)handle, &dcb ) )
    {
#if 1
      COMMTIMEOUTS ct;
      memset( &ct, 0, sizeof(ct) );

      ct.ReadIntervalTimeout = MAXDWORD;
      ct.ReadTotalTimeoutMultiplier = MAXDWORD;
      ct.ReadTotalTimeoutConstant = MAXDWORD-1;
      default_timeout_ms = waitRead_ms;
      if( SetCommTimeouts( (HANDLE)handle, &ct ) )
      {
        res = true;
      }
      else errSDK_SOURCE( ErrSource() );
#else
      res = true;
#endif
    }
    else errSDK_SOURCE( ErrSource() );
  }
  unlock();
  return res;;
}
//------------------------------------------------------------------------------
// Очистить очереди порта
// input -  входную очередь
// output - выходную очередь
/*void TComPort::clearFIFO( bool input, bool output )
{
  lock();
  if( handle != (int)INVALID_HANDLE_VALUE )
  {
    DWORD action	= 0;
    if( input )   action |= PURGE_RXCLEAR;
    if( output )  action |= PURGE_TXCLEAR;
    PurgeComm( (HANDLE)handle, action );
  }
  unlock();
}*/
//------------------------------------------------------------------------------
//==============================================================================
//==============================================================================
//==============================================================================
#else
//==============================================================================
//==============================================================================
//==============================================================================

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <asm/ioctls.h>
#include <sys/types.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <glib.h>
#include <pthread.h>
//------------------------------------------------------------------------------
// пустой обработчик, т.к. при по умолчанию при получении сигнала SIGUSR1
// приложение завершает работу
static void signalEvent(int) {}
//------------------------------------------------------------------------------
TComPort::TComPort( ):cs("TComPort")
{
//  mt_control = _mt_control;
  errSource = NULL;
  portNumber = -1;
  handle = -1;
  hasThreadID = false;
  cntWrited = cntReaded = 0;
  signal( SIGUSR1, signalEvent );
}
//------------------------------------------------------------------------------
TComPort::~TComPort()
{
}
//------------------------------------------------------------------------------
void TComPort::close()
{
  lock();
  if( handle != -1 )
  {
    ::close( handle );
    handle = -1;
  }
  if( autoErrSource ) errSource = NULL;
  unlock();
}
//------------------------------------------------------------------------------
// Открытие порта
bool TComPort::open( int pn )
{
  // Закрытие предыдущего порта
  close();
  bool ret = false;
  lock();
  portNumber = pn;
  // Запись номера порта в строке
  char str[20];
  if ( pn < 3 )
    sprintf( str, "/dev/ttyS%i", pn-1 );
  else
    sprintf( str, "/dev/ttyP%i", pn+1 );

  // Попытка открытия устройства
  // O_SYNC - ввод/вывод в синхронном режиме. write не вернет управление, пока
  // данные не будут физически выгружены в канал
  handle = ::open( str, O_SYNC | O_RDWR | O_NOCTTY );// | O_NDELAY );

  if ( handle != -1 )
  {
    eventAlert = 0;
    hasThreadID = false;
 		//errCount = 0;
    ret = true;
  }
  else
    errSTD_SOURCE( ErrSource() );
  unlock();
  return ret;
}
//------------------------------------------------------------------------------
bool TComPort::isOpen()
{
  return handle != -1;
}
//------------------------------------------------------------------------------
void TComPort::errRegister( const char *file, int line, guint32 errCode )
{
  if( !errCode ) errCode = errno;
  errSTD_Set( errCode, file, line, ErrSource() );
}
//------------------------------------------------------------------------------
// Запись в порт
bool TComPort::write( const void* lpvoid, unsigned long sz )
{
  bool ret = false;

  lock();
  if ( handle != -1 )
  {
    int res = ::write( handle, lpvoid, sz );
    if( res == -1 )
		{
      ERR_REGLOG();
		}
    else
      if( res != (int)sz )
        errRegisterStr( "Неполная запись в порт", __FILE__, __LINE__ );
      else
      {
        ret = true;
        g_atomic_int_add( (gint*)&cntWrited, sz );
      }
  }
  unlock();

  return ret;
}
//------------------------------------------------------------------------------
// Чтение из порта
int TComPort::read(void* lpvoid, unsigned long& sz, long timeout )
{
  int ret = rr_Error;
  lock();
l_repeat:
  if ( handle != -1 )
  {
    if( eventAlert )
    {
      eventAlert = 0;
      unlock();
      return rr_Alert;
    }
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET( handle, &rfds );

    if( !hasThreadID )
    {
      threadID = pthread_self();

      sigset_t  threadSigMask;
      sigemptyset( &threadSigMask );
      sigaddset( &threadSigMask, SIGUSR1 );
      pthread_sigmask( SIG_BLOCK, &threadSigMask, &origMask );
      hasThreadID = true;
    }

    if( timeout == -1 ) timeout = default_timeout_ms;
    struct timespec tv;
    if( timeout != -1 )
    {
      tv.tv_sec   = timeout / 1000;
      tv.tv_nsec  = (timeout%1000) * 1000000;
    }
    unlock();
    int s = pselect( handle + 1, &rfds, 0, 0, (timeout != -1) ? &tv : NULL, &origMask );
    if( s == 0 )
    {
      ret = rr_Timeout;
    }
    else
      if( s < 0 )
      {
        if( errno == EINTR )
        {
          lock();
          goto l_repeat;
        }
        else ERR_REG();
      }
      else
      {
        int n = 0;
        if( ioctl( handle, FIONREAD, &n ) == -1 )
          ERR_REG();
        else
          if( n )
          {
            if( n > (int) sz )
              n = sz;
            int r = ::read( handle, lpvoid, n );
            if( r == -1 )
              ERR_REG();
            else
              if( r != 0 )
              {
                sz = (unsigned long)r;
                g_atomic_int_add( (gint*)&cntReaded, sz );
                ret = rr_Read;
              }
          }
          else ret = rr_Timeout;
      }
  }
  else unlock();
  return ret;
}
//------------------------------------------------------------------------------
// Уведомить о "тревожном" событии
void TComPort::setAlert()
{
  lock();
  eventAlert = 1;
  if( hasThreadID )
  {
    unlock();
    pthread_kill( threadID, SIGUSR1 );
  }
  else unlock();
}
//------------------------------------------------------------------------------
bool TComPort::isFlag( int flag )
{
  bool res = false;
  lock();
  if( handle != -1 )
  {
    int rez;
    ioctl( handle, TIOCMGET, &rez );
    if( rez & flag )
      res = true;
  }
  unlock();
  return res;
}
//------------------------------------------------------------------------------
bool TComPort::isDSR()
{
  return isFlag( TIOCM_DSR );
}
//------------------------------------------------------------------------------
bool TComPort::isCTS()
{
  return isFlag( TIOCM_CTS );
}
//------------------------------------------------------------------------------
bool TComPort::setFlag( bool set, int flag )
{
  bool res = false;
  lock();
  if( handle != -1 )
  {
    int rez;
    ioctl( handle, TIOCMGET, &rez );
    if ( set )
      rez |= flag;
    else
      rez &= ~flag;
    ioctl( handle, TIOCMSET, &rez );
    res = true;
  }
  unlock();
  return res;
}
//------------------------------------------------------------------------------
bool TComPort::setRTS( bool set )
{
  return setFlag( set, TIOCM_RTS );
}
//------------------------------------------------------------------------------
bool TComPort::setDTR( bool set )
{
  return setFlag( set, TIOCM_DTR );
}
//------------------------------------------------------------------------------
static int get_speed( unsigned long s )
{
  if ( s < 50 )           return B0;
  else if ( s < 75 )      return B50;
  else if ( s < 110 )     return B75;
  else if ( s < 134 )     return B110;
  else if ( s < 150 )     return B134;
  else if ( s < 200 )     return B150;
  else if ( s < 300 )     return B200;
  else if ( s < 600 )     return B300;
	else if ( s < 1200 )    return B600;
	else if ( s < 1800 )    return B1200;
	else if ( s < 2400 )    return B1800;
	else if ( s < 4800 )    return B2400;
	else if ( s < 9600 )    return B4800;
	else if ( s < 19200 )   return B9600;
	else if ( s < 38400 )   return B19200;
	else if ( s < 57600 )   return B38400;
	else if ( s < 115200 )  return B57600;
	else if ( s < 230400 )  return B115200;
	else if ( s < 460800 )  return B230400;
	else if ( s < 921600 )  return B460800;
	else                    return B921600;
}
//------------------------------------------------------------------------------
// Установка состояния порта
bool TComPort::setPortState(
 	unsigned long baudRate,     // скорость
  unsigned char byteSize,     // кол-во бит
  unsigned char parity,       // четность
  unsigned char stopBits,     // кол-во стоповых бит
  unsigned char isCTS_RTS,    // аппаратное управление потоком
  long waitRead_ms   // ожидание чтения, мс
)
{
  if ( handle == -1 )
    return false;
  lock();
  termios tio;
  memset( &tio, 0, sizeof(tio) );
  // Включить прием
  tio.c_cflag |= CREAD;
  // Выключить управление модемными линиями при закрытии устройства
  tio.c_cflag |= HUPCL;
  //
  tio.c_cflag |= CLOCAL;
  //
  //tio.c_iflag |= /*IGNBRK |*/ IXON | IXOFF;

  // Количество бит
  tio.c_cflag |= ( byteSize == 8 ) ? CS8 : CS7;
  // Четность
  switch( parity )
  {
    case 0: // Нет проверки четности
      tio.c_iflag |= IGNPAR;
      break;
    case 1: // Проверка четности
      tio.c_cflag |= PARENB;
      break;
    case 2: // Проверка нечетности
    default:
      tio.c_cflag |= PARODD;
      break;
  }
  // Количество стоповых бит
  if( stopBits )
    tio.c_cflag |= CSTOPB; // Два стоп бита

  // аппаратное управление потоком
  if( isCTS_RTS )
    tio.c_cflag |= CRTSCTS;

  // Установка скорости
  int speed = get_speed( baudRate );
  cfsetospeed( &tio, speed );
  cfsetispeed( &tio, speed );

  // очистить входной буфер
  tcflush( handle, TCIFLUSH );

  // Установить назначенные параметры немедленно
  tcsetattr( handle, TCSANOW, &tio );

  default_timeout_ms = waitRead_ms; // ожидание чтения, мс
  unlock();
  return true;
}
//------------------------------------------------------------------------------
/*
// Очистить очереди порта
// input -  входную очередь
// output - выходную очередь
void TComPort::clearШтFIFO( bool input, bool output )
{
  lock();
  if ( handle != -1 )
  {
    int action = 0;
    if( input )   action |= TCIFLUSH;
    if( output )  action |= TCOFLUSH;
    tcflush( handle, action );
  }
  unlock();
}
*/
//------------------------------------------------------------------------------
// Установка таймаутов
/*bool TComPort::SetRS232Timeouts(
 DWORD rChar,
  DWORD rStr,
   DWORD rDep,
    DWORD wStr,
     DWORD wDep )
{
  read_timeout_mks = rDep * 1000;
    // Определяет константу, в миллисекундах,
    // используемую для вычисления общий период задержки
    // для операций чтения. Для каждого чтения, эта величина
    // добавлена к результату ReadTotalTimeoutMultiplier и
    // попрошенному количеству байтов.
    ct.ReadTotalTimeoutConstant = rDep;


  }
  return true;
}*/
#endif
//==============================================================================
//==============================================================================
//==============================================================================
void* TComPort::ErrSource()
{
  if( !errSource )
  {
    char str[20];
//    wsprintf( str, "Порт СОМ%d", portNumber );
    sprintf( str, "Порт СОМ%d", portNumber );
    autoErrSource = true;
#ifdef BL_LocalH  //GBaseLin 4.0
    errSource = err_RegisterSource( str, CP_WIN );
#else             //GBaseLin 3.0
    errSource = err_RegisterSource( str );
#endif
  }
  return errSource;
}
//------------------------------------------------------------------------------
void TComPort::errRegisterStr( const char *msg, const char *file, int line )
{
  errStr_Set( BL_CP_WIN, msg, file, line, ErrSource() );
}
//------------------------------------------------------------------------------
/**
 * Очищает входные буферы порта
 */
void TComPort::clearInputQueue(void)
{
  lock();
#if TD_TARGET != TD_LINUX

  if( handle != (int)INVALID_HANDLE_VALUE )
    PurgeComm( (HANDLE)handle, PURGE_RXCLEAR );

#else /* if LINUX */

  if ( handle != -1 )
    tcflush( handle, TCIFLUSH );

#endif // linux
  unlock();
}
//------------------------------------------------------------------------------
void TComPort::PrintLog( const void* buffer, unsigned long size, bool outStream )
{
  if( !mLog.name() )
  {
    char name[20];
    sprintf( name, "COM%d", portNumber );
    mLog.setName( name );
    mLog.setConsole( "c:\\logLS.txt" );   //###
  }

  if( mLog.checkEnabled() )
  {
    char comment[20];
    sprintf( comment, "at %d %s", (int)time(0), outStream ? " in" : "out" );
    mLog.hexA( comment, buffer, size );
    //mLog.printfA( (const char*)buffer ); //###
  }
}
//------------------------------------------------------------------------------

