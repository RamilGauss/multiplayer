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
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


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
