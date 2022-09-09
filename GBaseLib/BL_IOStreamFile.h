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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


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
