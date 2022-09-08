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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


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

