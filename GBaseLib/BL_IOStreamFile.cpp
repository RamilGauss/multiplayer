#include "BL_IOStreamFile.h"
#include <glib/gatomic.h>

#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

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
// ���������� ������ �����
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
// ������� ����
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
// ������� ���� ��� ������� �����
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
  //goBottom();  // ��������� � ����� �����
  return true;
}
//---------------------------------------------------------------
// ������� ������������ ����
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
// ������������ �������� ��������� �����������
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
// ������� ������ � ������ ����� ����������
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
// ������� ����
bool TIOStreamFile::deleteFile()
{
  if( !unlink( mNameFile ) ) return true;
  errSTD_SOURCE( errSource() );
  return false;
}
//---------------------------------------------------------------
// ��������� ������ �� ������
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
// �������� ������ � �����
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
// ������� ������� ��������� ��� -1
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
// ��������� � ������ �����
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
// ��������� � ����� �����
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
// ��������� � �������� �������
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
// �������� ���������
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
// ������ �� ������ � ������
bool TIOStreamFile::isOpen() const
{
  return mFile != -1;
}
//---------------------------------------------------------------
// ����� �����
long TIOStreamFile::length()
{
  return filelength( mFile );
}
//---------------------------------------------------------------
// �������� ������ � �����
bool TIOStreamFile::flush()
{
  return true;
}
//---------------------------------------------------------------
// ���������� ������ �����
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
