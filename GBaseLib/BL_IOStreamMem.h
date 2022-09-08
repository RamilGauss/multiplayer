#ifndef BL_IOStreamMemH
#define BL_IOStreamMemH

#include "BL_IOStream.h"

class GBASELIB_EI TIOStreamMem : public TIOStream
{
public:
  TIOStreamMem(void);
  virtual ~TIOStreamMem(void) { close(); }

  // Открыть файл в памяти фиксированного размера
  // file  - файл
  // sizefile - размер файла 
  TIOStreamMem( void* file, long sizefile );

  // Открыть файл в памяти фиксированного размера
  // file  - файл
  // sizefile - размер файла 
  bool Open( void* file, long sizefile );

  // Создать файл в памяти
  // sizebuf - размер буфера 
  // buffer  - внешний буфер (при удалении объекта не уничтожается)
  //           если внешний буфер не предоставлен, то создается объектом самостоятельно
  // canResized  - разрешение увеличивать начальный размер буфера 
  //           (только в случае работы с собственным буфером)
  bool Create( long sizebuf = 0, void* buffer = NULL, bool canResized = true );

  // Закрыть файл
  void close();
  
  // Прямой доступ к данным файла в памяти
  void* Ptr() { return mIsOpen ? mBuf : NULL; }

public: // реализация интерфейса TIOStream

  virtual void* errSource() const       { return mErrSource; }
  virtual void setErrSource( void* es ) { mErrSource = es; }

  // Чтение/запись данных
  virtual int read( void* buf, guint size );        // Прочитать данные из потока
  virtual int write( const void* buf, guint size ); // Записать данные в поток

  // навигация по потоку
  virtual long curPos( );             // текущая позиция указателя или -1
  virtual bool goTop( );              // указатель в начало файла
  virtual bool goPos( long pos );     // указатель в заданную позицию
  virtual bool goBottom( );           // указатель в конец файла
  virtual bool shiftPos( long offset );// сместить указатель

  // прочие методы
  virtual bool isOpen() const;          // открыт ли доступ к потоку
  virtual long length();                // длина файла
  virtual bool chsize( long size );     // установить размер файла

protected:
  bool    mIsOpen;    // признак создания файла
  char*   mBuf;       // буфер
  long    mFileSize;  // размер файла
  long    mBufSize;   // размер буфера ( mBufSize >= mFileSize )
  bool    mSelfBuf;   // признак освобождения памяти при удалении объекта
  bool    mCanResized;// разрешение увеличивать размер буфера
  //long    mDelta;     // шаг приращения файла

  long    mPos;       // смещение текущего указателя от начала файла
  void*   mErrSource;

};

#endif // IOStreamMemH
