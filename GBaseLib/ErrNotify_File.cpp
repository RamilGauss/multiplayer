#include "ErrorReg.h"
//#include "OutDisk.h"
#include "BL_AppFile.h"
#include <string.h>
#if defined( __GNUC__ )
  #include <unistd.h>
  #define O_BINARY      0
#else
  #include <io.h>
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

//static outDisk fileLogErr;
static int fileLogErr = -1;
static char fileName[MAX_PATH];  
//-----------------------------------------------------------------------------
static void NotificationError( void* );

// �������� �������� ������ ������������� ������
static TDefNotification defReg =
{
  NotificationError,          // ��������� ������
  0,                          // �������� ��������� ������
  NULL                        // ��������� ����������� � ������
};

//-----------------------------------------------------------------------------

// ����� �������� ������ � ����� �����������
static void writeToFile( TErrorDesc *desc )
{
  if( fileLogErr == -1 )
  {
#ifdef TD_WINDOWS
    unsigned createmode = S_IREAD | _S_IWRITE;
#else
    unsigned createmode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH; // 0x777 ?
#endif // TD_WINDOWS
    fileLogErr = open( fileName, O_CREAT|O_APPEND|O_WRONLY, createmode );
    if( fileLogErr == -1 )
      return;
  }
  write( fileLogErr, desc->str, desc->length );
  write( fileLogErr, "\xD\xA", 2 );
}
//-----------------------------------------------------------------------------
void NotificationError( void* )
{
  // ��������� ��������� �������� ���������� ������
  err_LastDesc_Fill();
  
  // ������� ��� � ����
  writeToFile( &err_LastDesc );
  
  // �������� ����������� �� ������ ���������� �����������
  if( defReg.next ) defReg.next->proc( defReg.next->user_data );
}
//-----------------------------------------------------------------------------

void ErrNotify_File_Init( const char* _fileName )
{
  ErrNotify_File_Done();
  if( _fileName )
    strcpy( fileName, _fileName );
  else
    BL_AppFile_Ext( fileName, ".err" );
 
  err_SetNotification( &defReg, true );
}
//-----------------------------------------------------------------------------
// �������� ������������ ������
void ErrNotify_File_Done()
{
  //fileLogErr.close();
  if( fileLogErr != -1 )
    ::close( fileLogErr );
  err_SetNotification( &defReg, false );
}
//-----------------------------------------------------------------------------
