#ifndef BL_IOStreamFileH
#define BL_IOStreamFileH

#include "BL_IOStream.h"

/*
      TIOStreamFile - реализация абстрактного класса TIOStream применительно к файлам.
  Имена файлов должны указываться в локальной кодировке.

      TIOStreamFile создаёт/открывает файл с начальным количеством ссылок равным 1. 
  Методы ref() и unref() могут использоваться для увеличения и уменьшения количества ссылок 
  соответственно. Когда количество ссылок достигло 0, файл закрывается.
*/

class GBASELIB_EI TIOStreamFile : public TIOStream
{
public:
  TIOStreamFile(void);
  virtual ~TIOStreamFile(void) { close(); }

  // Дескриптор файла
  int handle() const { return mFile; }
public:

  // создать файл
  // fname - название файла в локальной кодировке
  // read  - разрешение на чтение из файла
  // write - разрешение на запись в файл
  // append- запись в файл в режиме "всегда дописываем к файлу"
  bool create( const char* fname, bool read = false, bool write = true, bool append = false );

  // открыть файл или создать новый
  // fname - название файла в локальной кодировке
  // read  - разрешение на чтение из файла
  // write - разрешение на запись в файл
  // append- запись в файл в режиме "всегда дописываем к файлу"
  bool open( const char* fname, bool read = true, bool write = true, bool append = false );

  // открыть существующий файл
  // fname - название файла в локальной кодировке
  // read  - разрешение на чтение из файла
  // write - разрешение на запись в файл
  // append- запись в файл в режиме "всегда дописываем к файлу"
  bool openExist( const char* fname, bool read = true, bool write = false, bool append = false );

  // сформировать дубликат файлового дескриптора
  int dup();

  // связать объект с файлом через дескриптор
  void link( int handle );

  // закрыть файл (отключиться от него)
  void close();
  
  // удалить файл
  bool deleteFile();

  const char* nameFile() const { return mNameFile; }
  void setNameFile( const char* nameFile );

public: // реализация интерфейса TIOStream

  // Управление источником отказов
  virtual void* errSource() const;
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
  virtual bool flush();                 // сбросить буфера в поток
  virtual bool chsize( long size );     // установить размер файла

protected:
  int   mFile;        // Дескриптор файла
  mutable void* mErrSource;
  char mNameFile[MAX_PATH];
};

#ifndef TD_WINDOWS
  // Определить размер файла
  long GBASELIB_EI filelength( int fd );
#endif


#endif // IOStreamFileH
