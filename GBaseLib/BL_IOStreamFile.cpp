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


#include "BL_IOStreamFile.h"
#include <glib/gatomic.h>

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


#ifdef _MSC_VER
# pragma warning(push)
# pragma warning( disable : 4996 )
#endif


#if defined( __GNUC__ )
  #include <unistd.h>
  #define O_BINARY      0
#else
  #include <io.h>
#endif

#include "ErrorReg.h"
#include "BL_Conv.h"

//---------------------------------------------------------------
#if defined( __GNUC__ )
// Определить размер файла
long filelength( int fd )
{ 
  long oldval = lseek( fd,0,SEEK_CUR);
  if( oldval == -1L ) return -1L;
  long retval = lseek( fd, 0, SEEK_END );
  if( retval == -1L ) return -1L;
  if( lseek( fd, oldval, SEEK_SET )== -1L ) return -1L;
  return retval;
}
#endif
//---------------------------------------------------------------
TIOStreamFile::TIOStreamFile(void)
{
  mFile = -1;
  mErrSource = NULL;
  mNameFile[0] = '\0';
}
//---------------------------------------------------------------
void* TIOStreamFile::errSource() const       
{ 
  if( !mErrSource && mNameFile[0] )
    mErrSource = err_RegisterSource( mNameFile );
  return mErrSource; 
}
//---------------------------------------------------------------
void TIOStreamFile::setNameFile( const char* nameFile )
{
  strcpy( mNameFile, nameFile );
}
//---------------------------------------------------------------
static void setRWFlag( int &access, unsigned &mode, bool read, bool write, bool append )
{
  access |= O_BINARY;
  if( read )
  {
    if( write ) {
      access |= O_RDWR;
      if( append ) access |= O_APPEND;
      mode |= S_IWRITE;
    }
    else access |= O_RDONLY;
  }
  else
    if( write ) {
      access |= O_WRONLY;
      if( append ) access |= O_APPEND;
      mode |= S_IWRITE;
    }
}
//---------------------------------------------------------------
// создать файл
bool TIOStreamFile::create( const char* fname, bool read, bool write, bool append )
{
  close();
  strcpy( mNameFile, fname );
  int access = O_CREAT|O_TRUNC;
#ifdef TD_WINDOWS
  unsigned createmode = S_IREAD | _S_IWRITE;
#else
  unsigned createmode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // 0x777 ?
#endif // TD_WINDOWS
  setRWFlag( access, createmode, read, write, append );
  mFile = ::open( fname, access, createmode );
  if( mFile == -1 ) {
    errSTD_SOURCE( errSource() );
    return false;
  }
  return true;
}
//---------------------------------------------------------------
// открыть файл или создать новый
bool TIOStreamFile::open( const char* fname, bool read, bool write, bool append )
{
  close();
  strcpy( mNameFile, fname );
  int access = O_CREAT;
#ifdef TD_WINDOWS
  unsigned createmode = S_IREAD | _S_IWRITE;
#else
  unsigned createmode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // 0x777 ?
#endif // TD_WINDOWS
  setRWFlag( access, createmode, read, write, append );
  mFile = ::open( fname, access, createmode );
  if( mFile == -1 ) {
    errSTD_SOURCE( errSource() );
    return false;
  }
  //goBottom();  // указатель в конец файла
  return true;
}
//---------------------------------------------------------------
// открыть существующий файл
bool TIOStreamFile::openExist( const char* fname, bool read, bool write, bool append )
{
  close();
  int access = 0;
  unsigned mode = 0;
  setRWFlag( access, mode, read, write, append );
  mFile = ::open( fname, access, mode );
  if( mFile == -1 ) {
    errSTD_SOURCE( errSource() );
    return false;
  }
  return true;
}
//---------------------------------------------------------------
// сформировать дубликат файлового дескриптора
int TIOStreamFile::dup()
{
  if( mFile != -1 )
  {
    int res = ::dup( mFile );
    if( res != -1 ) return res;
    errSTD_SOURCE( errSource() );
  }
  return -1;
}
//---------------------------------------------------------------
// связать объект с файлом через дескриптор
void TIOStreamFile::link( int handle )
{
  close();
  mFile = handle;
}
//---------------------------------------------------------------
void TIOStreamFile::close()
{
  if( mFile != -1 )
  {
    ::close( mFile );
    mFile = -1;
  }
  mErrSource = NULL;
}
//---------------------------------------------------------------
// удалить файл
bool TIOStreamFile::deleteFile()
{
  if( !unlink( mNameFile ) ) return true;
  errSTD_SOURCE( errSource() );
  return false;
}
//---------------------------------------------------------------
// Прочитать данные из потока
int TIOStreamFile::read( void* buf, guint size )
{
  int res = ::read( mFile, buf, size );
  if( res < 0 )
  {
    errSTD_SOURCE( errSource() );
    return -1;
  }
  return res;
}
//---------------------------------------------------------------
// Записать данные в поток
int TIOStreamFile::write( const void* buf, guint size )
{
  int res = ::write( mFile, buf, size );
  if( res < 0 )
  {
    errSTD_SOURCE( errSource() );
    return -1;
  }
  return res;
}
//---------------------------------------------------------------
// текущая позиция указателя или -1
long TIOStreamFile::curPos( )
{
  long res = lseek( mFile, 0, SEEK_CUR ); 
  if( res == -1 )
  {
    errSTD_SOURCE( errSource() );
    return -1;
  }
  return res;
}
//---------------------------------------------------------------
// указатель в начало файла
bool TIOStreamFile::goTop( )
{
  long res = lseek( mFile, 0, SEEK_SET );
  if( res == -1 )
  {
    errSTD_SOURCE( errSource() );
    return false;
  }
  return true;
}
//---------------------------------------------------------------
// указатель в конец файла
bool TIOStreamFile::goBottom( )
{
  long res = lseek( mFile, 0, SEEK_END );
  if( res == -1 )
  {
    errSTD_SOURCE( errSource() );
    return false;
  }
  return true;
}
//---------------------------------------------------------------
// указатель в заданную позицию
bool TIOStreamFile::goPos( long pos )
{
  long res = lseek( mFile, pos, SEEK_SET );
  if( res != pos )
  {
    errSTD_SOURCE( errSource() );
    return false;
  }
  return true;
}
//---------------------------------------------------------------
// сместить указатель
bool TIOStreamFile::shiftPos( long offset )
{
  long res = lseek( mFile, offset, SEEK_CUR );
  if( res == -1 )
  {
    errSTD_SOURCE( errSource() );
    return false;
  }
  return true;
}
//---------------------------------------------------------------
// открыт ли доступ к потоку
bool TIOStreamFile::isOpen() const
{
  return mFile != -1;
}
//---------------------------------------------------------------
// длина файла
long TIOStreamFile::length()
{
  return filelength( mFile );
}
//---------------------------------------------------------------
// сбросить буфера в поток
bool TIOStreamFile::flush()
{
  return true;
}
//---------------------------------------------------------------
// установить размер файла
bool TIOStreamFile::chsize( long size )
{
#ifdef TD_WINDOWS
  int res = ::chsize( mFile, size ); 
#else
  int res = ::ftruncate( mFile, size ); 
#endif
  if( res == -1 )
  {
    errSTD_SOURCE( errSource() );
    return false;
  }
  return true;
}
//---------------------------------------------------------------

#ifdef _MSC_VER
# pragma warning(pop)
#endif
