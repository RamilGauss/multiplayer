/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning( disable : 4996 )
#endif


#include "LogManager.h"
#include "BL_AppFile.h"
#include "LogFile.h"
#include "BL_ConfigFile.h"
#include <glib.h>
#include <string.h>

TAutoCreateVarT<TLogManager> LogManager;

#define GRP_NAME        "LOG"

#if TD_TARGET == TD_LINUX
  #define stricmp strcasecmp
#endif

#ifdef TD_WINDOWS
  #include <windows.h>
#include <list>
#endif

//-----------------------------------------------------------------------------
TLogManager::TLogManager()
{
#ifdef TD_WINDOWS
  if( !GetConsoleWindow() ) return;
#endif  
  Register( &mLogStdOut );
  Register( &mLogStdErr );
}
//-----------------------------------------------------------------------------
TLogManager::~TLogManager()
{
  for( TListConsole::iterator item = mConsoles.begin(); item != mConsoles.end(); )
  {
    const char* name = (*item)->name();
    if( name[0] != ':' )
    {
      delete *item;
      item = mConsoles.erase( item );
    }
    else
      item++;
  }
}
//-----------------------------------------------------------------------------
ILogConsole* TLogManager::Find( const char* name )
{
  for( TListConsole::const_iterator item = mConsoles.begin(); item != mConsoles.end(); item++ )
  {
    if( stricmp( (*item)->name(), name ) == 0 ) 
      return *item;
  }
  return NULL;
}
//-----------------------------------------------------------------------------
void TLogManager::Register(ILogConsole* console)
{
  lock();
  mConsoles.push_front( console );
  unlock();
}
//-----------------------------------------------------------------------------
void TLogManager::Unregister(ILogConsole* console)
{
  lock();
  for( TListWriter::const_iterator item = mWriters.begin(); item != mWriters.end(); item++ )
    (*item)->disableConsole( console );

//  for( TListConsole::const_iterator item = mConsoles.begin(); item != mConsoles.end(); item++ )
//    (*item)->disableConsole( console );

  mConsoles.remove( console );
  unlock();
}
//-----------------------------------------------------------------------------
void TLogManager::Link( TBL_Log* writer )
{
  if( !writer || !writer->name() ) return;

  lock();
  // ���� ������ �������� "��������" �� ����, �� ��� ������������� ���� ��� ���������.
  // ��� ��������� �������� ���������� "��������" ������ ����������� ������, 
  // ������ �� ����� ��� �������������.
  if( !writer->mListConsole.empty() )
  {
    unlock();    
    return;
  }

  setGroupComment();
  char st[MAX_PATH];

  // ������������ �������� ����� �� ���������
  if( stricmp( writer->name(), APP_LOG_NAME ) == 0 )
  {
    // ��� ������ ���-����� ���������� - <����� ����������>.log
    BL_AppFile_Ext( st, ".log" );
    char* bn = g_path_get_basename( st );
    if( bn )
    {
      strcpy( st, bn );         // � ����
      strcat( st, " :strerr" ); // � �� �����
      g_free( bn );
    }
    else strcpy( st, "off" );
  }
  else strcpy( st, "off" );

  // ������ ���������� "��������" �� ini-�����
  char *consoleDesc = AppConfigFile->GetValue( GRP_NAME, writer->name(), st );
  parseDescConsole( writer, consoleDesc ? consoleDesc : "" );
  if( consoleDesc ) g_free(consoleDesc);
  mWriters.push_front( writer );
  unlock();
}
//-----------------------------------------------------------------------------
void TLogManager::parseDescConsole( TBL_Log* writer, const char* descConsole )
{
  ILogConsole* result = NULL;
	bool setOff = false;
  gchar** arrStr = g_strsplit( descConsole, " ", -1 );
  for( int i = 0; arrStr[i] && arrStr[i][0]; i++ )
  {
    char* item = arrStr[i];
    if( stricmp( item, "off") == 0 )
    {
      writer->suspend();
			setOff = true;
    }
    else
    {
      ILogConsole* console;
      if( item[0] == ':' ) // ��� ����������� �������
      {
        console = Find( item );
      }
      else   // ��� ����-�������
      {
        char st[MAX_PATH];
        // ��������� ��� ���-����� �� ����������� ��� �������������� �����
        if( g_path_is_absolute( item ) )
          strcpy( st, item );
        else
          BL_AppDir_Append( st, item, 0 );

        console = Find( st );
        if( !console )
        {
          console = new TLogFile( st, true );
          mConsoles.push_back( console );
        }
      }
			if( console )
			{
        writer->addConsole( console );
				result = console;
			}
    }
  }
  g_strfreev( arrStr );

	// ������� �� ������ � �� ������ ����������
  if( !result && !setOff )
  {
    // ��� ����� ��� ����������?
    if( stricmp( writer->name(), APP_LOG_NAME ) == 0 )
    { // ��� ���-����� ����������:
      char st[MAX_PATH];
      BL_AppFile_Ext( st, ".log" );                    // ��� �� ���������
      result = new TLogFile( st, true );
      mConsoles.push_front( result );                  // �����������
      writer->addConsole( result );
    }
    else 
    {
      AppLog.checkEnabled();            // ���������� ����� ��� ����������
      //result = AppLog.mConsole;         // ��������� �� ��� ������� 
      writer->addConsole( AppLog.mListConsole );
    }
  }
//  writer->mConsole = result;
//  return result != NULL;
}
//-----------------------------------------------------------------------------
void TLogManager::Link( TBL_Log* writer, const char* descConsole )
{
  if( !descConsole || descConsole[0]=='\0' ) return Link( writer );

  lock();
  parseDescConsole( writer, descConsole );
  mWriters.push_front( writer );
  unlock();
}
//-----------------------------------------------------------------------------
void TLogManager::Unlink( TBL_Log* writer )
{
  lock();
  mWriters.remove( writer );
  unlock();
}
//-----------------------------------------------------------------------------
ILogConsole* TLogManager::CheckConsole( const char* consoleName )
{
  ILogConsole* console;
  if( consoleName[0] == ':' ) // ��� ����������� �������
  {
    console = Find( consoleName );
  }
  else   // ��� ����-�������
  {
    char st[MAX_PATH];
    // ��������� ��� ���-����� �� ����������� ��� �������������� �����
    if( g_path_is_absolute( consoleName ) )
      strcpy( st, consoleName );
    else
      BL_AppDir_Append( st, consoleName, 0 );

    console = Find( st );
    if( !console )
    {
      console = new TLogFile( st, true );
      mConsoles.push_back( console );
    }
  }
  return console;
}
//-----------------------------------------------------------------------------
// ������������ ����������� � ������ ����������������� �����
void TLogManager::setGroupComment()
{ 
  bool hasComment;
  {
    char *comment = AppConfigFile->GetComment( GRP_NAME, NULL );
    hasComment = comment && strlen(comment);
    g_free( comment );     
  }

  if( !hasComment ) // ����������� ��� �� �����
  {
    const char commentW[] = {
      "---------------------------------------------------------\n"
      "------    ������ ���������� �������� �����������   ------\n"
      "---------------------------------------------------------\n"
      "��������� ���������� ������ LOG: \n"
      "   <�������� ������>=<�������� ������>, ��� \n"
      " \n"
      "����������� ������ �����������: \n"
      "   APP - ����� ���-����� ����������. ���� �������� �� ������,  \n"
      "         �� ��������������� ���-���� � ��������� <��� ����������>.log \n"
      " \n"
      "<�������� ������> = [off] [�������] ..., ��� \n"
      "   off  - ������� ������������ ������ \n"
      "   ������� - �������� ������� ������ ������. �������� ����� ���� \n"
      "          ������� ��������� - ����� ����� ���������� �� ���. \n"
      "          ���� �������� ������� ���������� � ������� ':' �� ��� ��������� \n"
      "          �� \"�����������\" ������� - ����������� ������. \n"
      "          ����� �������� ������� ���������������� ��� ��� ���-�����. \n"
      "          ��� ���-����� ����� ���� � ������ ����� � ����� ��� � ����� \n"
      "          ������������ �������� ������� ����������.   \n"
      "          ���� ��� �� ������, �� ��������������� �������, �����������  \n"
      "          ��� ������������ ������ <APP>.\n"
      " \n"
      "����������� �������: \n"
      "   :infoLine - �������������� ������� ���������� (��� GUI ����������) \n"
      "   :stdout   - ��������� ������� ������ (�� ��������� - �������) \n"
      "   :stderr   - ��������� ������� �������(�� ��������� - �������) \n"
    };
    gsize bytes_written;
    gchar* commentL = g_convert( commentW, -1, bl_locale, BL_CP_WIN, NULL, &bytes_written, NULL );
    if( commentL )
    {
      AppConfigFile->SetComment( GRP_NAME, NULL, commentL );
      g_free( commentL );
    }
  }
}
//-----------------------------------------------------------------------------

#ifdef _MSC_VER
# pragma warning(pop)
#endif
