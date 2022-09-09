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


#ifdef _MSC_VER
# pragma warning(push)
# pragma warning( disable : 4996 )
#endif


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

// Описание элемента списка регистраторов ошибок
static TDefNotification defReg =
{
  NotificationError,          // процедура вызова
  0,                          // параметр процедуры вызова
  NULL                        // следующий регистратор в списке
};

//-----------------------------------------------------------------------------

// Вывод описания отказа в канал уведомителя
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
  // формируем текстовое описание последнего отказа
  err_LastDesc_Fill();
  
  // выводим его в файл
  writeToFile( &err_LastDesc );
  
  // передаем уведомление об отказе следующему обработчику
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
// Закрытие регистратора ошибок
void ErrNotify_File_Done()
{
  //fileLogErr.close();
  if( fileLogErr != -1 )
    ::close( fileLogErr );
  err_SetNotification( &defReg, false );
}
//-----------------------------------------------------------------------------

#ifdef _MSC_VER
# pragma warning(pop)
#endif
