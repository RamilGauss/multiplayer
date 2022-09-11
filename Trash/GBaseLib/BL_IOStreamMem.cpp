/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
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


#include "BL_IOStreamMem.h"
#include <glib/gmem.h>
#include <string.h>
#include "ErrorReg.h"

//---------------------------------------------------------------
TIOStreamMem::TIOStreamMem(void)
{
  mIsOpen = false;
  mErrSource = NULL;
}
//---------------------------------------------------------------
// Открыть файл в памяти фиксированного размера
TIOStreamMem::TIOStreamMem( void* buffer, long sizebuf )
{
  mIsOpen = false;
  mErrSource = NULL;
  Open( buffer, sizebuf );
}
//---------------------------------------------------------------
// Открыть файл в памяти фиксированного размера
bool TIOStreamMem::Open( void* buffer, long sizebuf )
{
  close();
  if( !buffer ) return false;
  mBuf = (char*)buffer;
  mSelfBuf = false;
  mCanResized = false;
  mBufSize = sizebuf;
  mFileSize = sizebuf;
  mPos = 0;
  mIsOpen = true;

  return true;
}
//---------------------------------------------------------------
void TIOStreamMem::close()
{
  if( !mIsOpen ) return;
  if( mSelfBuf )
    g_free( mBuf );
  mIsOpen = false;
}
//---------------------------------------------------------------
// Создать файл фиксированного размера
bool TIOStreamMem::Create( long sizebuf, void* buffer, bool canResized )
{
  close();
  if( buffer )
  {
    mBuf = (char*)buffer;
    mSelfBuf = false;
    mCanResized = false;
  }
  else
  {
    if( sizebuf )
    {
      mBuf = (char*)g_try_malloc( sizebuf );
      if( !mBuf ) 
      {
        errStr_SETSOURCE( "Невозможно создать файл (недостаточно памяти)", mErrSource );
        return false;
      }
    }
    else mBuf = NULL;
    mSelfBuf = true;
    mCanResized = canResized;
  }
  mBufSize = sizebuf;
  mFileSize = 0;
  mPos = 0;
  mIsOpen = true;

  return true;
}
//---------------------------------------------------------------
// Прочитать данные из потока
int  TIOStreamMem::read( void* buf, guint size )
{
  if( !mIsOpen ) 
  { 
    errStr_SETSOURCE( "Попытка операции с закрытым файлом", mErrSource ); 
    return -1; 
  }
  if( (mPos + (long)size) > mFileSize ) size = mFileSize - mPos;
  memcpy( buf, mBuf + mPos, size );
  mPos += size;
  return (int)size;
}
//---------------------------------------------------------------
// Записать данные в поток
int  TIOStreamMem::write( const void* buf, guint size ) 
{
  if( !mIsOpen ) 
  { 
    errStr_SETSOURCE( "Попытка операция с закрытым файлом", mErrSource ); 
    return -1; 
  }
  if( !size ) return 0;

  if( (mPos + (long)size) > mBufSize )
  {
    chsize( mPos + size );
    if( (mPos + (long)size) > mBufSize )
      size = mBufSize - mPos;
  }
  memcpy( mBuf + mPos, buf, size );
  mPos += size;
  if( mFileSize < mPos ) mFileSize = mPos;
  return (int)size;
}
//---------------------------------------------------------------
// текущая позиция указателя или -1
long TIOStreamMem::curPos( )
{
  return mPos;
}
//---------------------------------------------------------------
// указатель в начало файла
bool TIOStreamMem::goTop( )
{
  mPos = 0; return true;
}
//---------------------------------------------------------------
// указатель в заданную позицию
bool TIOStreamMem::goPos( long pos )
{
  if( (pos < 0) || (pos > mFileSize) ) 
  {
    errStr_SETSOURCE( "Попытка обращения за границы файла", mErrSource ); 
    return false; 
  }
  mPos = pos; 
  return true;
}
//---------------------------------------------------------------
// указатель в конец файла
bool TIOStreamMem::goBottom( )
{
  mPos = mFileSize; 
  return true;
}
//---------------------------------------------------------------
// сместить указатель
bool TIOStreamMem::shiftPos( long offset )
{
  return goPos( mPos + offset );
}
//---------------------------------------------------------------
// открыт ли доступ к потоку
bool TIOStreamMem::isOpen() const
{
  return mIsOpen;
}
//---------------------------------------------------------------
// длина файла
long TIOStreamMem::length()
{
  return mFileSize;
}
//---------------------------------------------------------------
// установить размер файла
bool TIOStreamMem::chsize( long size )
{
  if( !mIsOpen ) 
  { 
    errStr_SETSOURCE( "Попытка операция с закрытым файлом", mErrSource ); 
    return false; 
  }

  if( mBuf )
  {
    if( mFileSize >= size ) mFileSize = size;
    else // mFileSize необходимо увеличить:
    {
      if( mBufSize >= size ) mFileSize = size;
      else
      {
        if( !mCanResized ) 
        {
          mFileSize = mBufSize;
          return false;
        }
        else
        {
          long newSize = mBufSize * 2;
          while( newSize < size ) newSize *= 2;

          gpointer newPtr = g_try_realloc( mBuf, newSize );
          if( !newPtr )
          {
            errStr_SETSOURCE( "Недостаточно памяти для выполнения операции", mErrSource );
            return false;
          }
          mBuf = (char*)newPtr;
          mBufSize = newSize;
          mFileSize = size;
        }
      }
    }
  }
  else
  {
    if( mCanResized )
    {
      mBufSize = mFileSize = size;
      if( size )                                
        mBuf = (char*)g_try_malloc( size );
    }
    else return false;
  }
  if( mPos > mFileSize ) mPos = mFileSize;                      
  return true;
}
//---------------------------------------------------------------
