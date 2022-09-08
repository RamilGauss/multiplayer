#include <stdio.h>
#include "BL_AppFile.h"

//------------------------------------------------------------------------------
#if defined(_WIN32)//-----------------------------------------------------------
//------------------------------------------------------------------------------
#include <shlwapi.h>
#include "BL_Debug.h"
#include "ErrorReg.h"
#include <glib.h>
#include "BL_Conv.h"
#include "BL_AppFile.h"
#if defined(__BORLANDC__)
	#pragma link "shlwapi.lib"     
#endif
static FILE* ferr;
//-----------------------------------------------------------------------------
// Сообщение об ошибке зафиксированной в теле программы
// lpszFileName - имя файла где зафиксирована ошибка
// nLine - номер строки
//******************************************************************************
void BL_MessageBug( const char* lpszFileName, int nLine )
{
  static bool show = true;
  if( show )
  {
    char text[MAX_PATH];
    wsprintf( text, "%s, %d", lpszFileName, nLine );
    char caption[MAX_PATH];
    if( ::GetModuleFileName( NULL, caption, MAX_PATH ) )
      PathStripPath( caption );
    else
      lstrcpy( caption, "Зафиксирован сбой" );

    if( ::MessageBox( NULL, text, caption, MB_OKCANCEL | MB_SYSTEMMODAL | MB_TOPMOST ) != IDOK )
      show = false;
  }
}
//------------------------------------------------------------------------------
static bool prepareFERR()
{
  if( !ferr )
  {
    if( GetConsoleWindow() )
      ferr = stderr;
    else
    {
      char st[MAX_PATH];
      ferr = fopen( BL_AppFile_Ext( st, ".log" ), "w" );
    }
  }
	return ferr != 0;
}
//------------------------------------------------------------------------------
#else //defined(_WIN32)//-------------------------------------------------------
//------------------------------------------------------------------------------

#include "BL_Debug.h"
#include <glib.h>
#include "BL_Conv.h"
//------------------------------------------------------------------------------
void BL_MessageBug( const char* lpszFileName, int nLine )
{
  fprintf( stderr, "Fix bug: %s, %d\n", lpszFileName, nLine );
  fflush( stderr );
}
//------------------------------------------------------------------------------
#endif //TD_TARGET
//------------------------------------------------------------------------------

