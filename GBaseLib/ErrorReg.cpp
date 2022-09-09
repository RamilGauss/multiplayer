/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#include "ErrorReg.h"
#include <string.h>
#include <stdio.h>
#include <glib.h>
#include "BL_Debug.h"
#include "BL_Conv.h"

#if defined(TD_WINDOWS)
  #include <windows.h>
#endif

TErrorDef err_LastDef;                          
TErrorDesc err_LastDesc;

//------------------------------------------------------------------------------
struct TES_Heap
{
  GStringChunk*  heap;

  bool init()
  {
    if( !heap )
      heap = g_string_chunk_new( 4096 );
    return heap != NULL;
  }
  void done()
  {
    if( heap )
    {
      g_string_chunk_free(heap); 
      heap = NULL; 
    }
  }

  TES_Heap() { heap = NULL; }
  ~TES_Heap() { done(); }
};
static TES_Heap ES_Heap; 
//------------------------------------------------------------------------------
// ������ ������������������ ����� ������
#define DSP_SIZE  100
static PDefErrorGroup arrGroupProc[DSP_SIZE+1];
static guint8 countRegGroup = 0;
//------------------------------------------------------------------------------
static GStaticRecMutex CS;
#define begin()  g_static_rec_mutex_lock( &CS );
#define end()    g_static_rec_mutex_unlock( &CS );
//------------------------------------------------------------------------------
// ��������� ����������� ������� ������������� �� ������
static TDefNotification* pDefNotification;

// ���������/�������� �������� ������������
// pDef: ��������� ������������
// set:  true/false - ����������/�������
static void SetNotification( TDefNotification *pDef, bool set )
{
  // ���������� ����� �����������
  if( set )
  { // ������ ������������� �� ����
    if( pDefNotification )
    {
      // ���� �� ����� ������
      TDefNotification* p = pDefNotification;
      while( p->next ) p = p->next;
      // � ���������� ������������ ������������ �����
      p->next = pDef;
    }
    else pDefNotification = pDef; // ������� ������ ������� � ������
  }
  else // ������� ����������� �� ������
  {
    // ������ ������������� �� ����
    if( pDefNotification )
      if( pDefNotification == pDef ) // ������� ������ �����������
      {
        // ������������ ������ ������
        pDefNotification = pDefNotification->next;
      }
      else
      {
        // �� ������ �� ������ �� ��������� ����������� � ��������
        TDefNotification* p = pDefNotification;
        while( p->next )
        {
          if( p->next == pDef )
          {
            p->next = pDef->next;
            break;
          }
          p = p->next;
        }
      }
  }
}
//------------------------------------------------------------------------------
// ���������/�������� �������� ������������
// pDef: ��������� ������������
// set:  true/false - ����������/�������
void err_SetNotification( TDefNotification *pDef, bool set )
{
  begin()
  SetNotification( pDef, set );
  end()
}
//------------------------------------------------------------------------------
// ������������� ������� ����������� ������
void err_Init()
{
  pDefNotification = NULL;        // ��� �������� ������������

  // ������� ������ ������������������ �����
  memset( arrGroupProc, 0, sizeof(PDefErrorGroup)*DSP_SIZE );
  countRegGroup = 0;

  err_LastDef.group = 0;
  err_LastDef.code = 0;
  err_LastDesc.length = 0;

  // ��� ���������������� ������, ������� ����� ���������� ����������� ������
  if( !g_thread_supported() ) g_thread_init (NULL);
  g_static_rec_mutex_init( &CS );
}
//---------------------------------------------------------------------------
// ���������� ������ ������� ����������� ������
void err_Done()
{
  // ��� ���� ������������������ ����� ������ �������� ��������� ��������
  PDefErrorGroup* pDEG = arrGroupProc + 1;
  for( guint8 i = countRegGroup; i > 0; i--, pDEG++ )
    if( *pDEG && (*pDEG)->procDoneGroup ) (*pDEG)->procDoneGroup();

  g_static_rec_mutex_free( &CS );
  ES_Heap.done();
}
//------------------------------------------------------------------------------
// ���������������� ������
void err_SetError( guint8 group, size_t code, const char* file, int line, void* source )
{
  // ������� �������������������� ������
  if( !group )
  {
		fprintf( stderr, "err_Set: unknown group to %s\n", file );
    return;
  }

  begin()

  time( &err_LastDef.time ); // ��������� ��
  err_LastDef.group = group;
  err_LastDef.code = code;
  err_LastDef.file = file;
  err_LastDef.line = line;
  err_LastDef.source = source;
  
  err_LastDesc.length = 0;  // ���������� ���������� ���������������� �������� ������

  // ���� ���� ������� ����������� - ���������� ��� � ������������� ������
  if( pDefNotification )
  {
    pDefNotification->proc( pDefNotification->user_data );
  }
  else
  {
    char groupName[256];
    if( !err_GetStrError( group, code, groupName, sizeof(groupName) ) )
      strcpy( groupName, "�������������������� �����" );
#if defined(TD_WINDOWS)
    if( !GetConsoleWindow() )
    {
      char title[512];
      sprintf( title, "%s, %d",file, line );
      ::MessageBox( NULL, groupName, title, MB_OK | MB_TASKMODAL );
    }
    else
#endif
    {
		  char stOut[sizeof(groupName)*2];
      int lenDest = sizeof(stOut);
      bl_conv( bl_locale, stOut, lenDest, BL_CP_WIN, groupName );
      fprintf( stderr, "%s, <%s, %d>\n", stOut, file, line );
    }
  }
  end()
}
//------------------------------------------------------------------------------
// ���������������� ����� ������ ������
// pdeg - �������� �� ����������
// res: ��� ������ (���������� �� ����� �����������)
guint8 err_RegisterGroup( PDefErrorGroup pdeg )
{
  guint8 codeGroup = 0;
  begin()
  // ���� ������ ����� �� ���������� � ���������� ��������
  if( (countRegGroup < DSP_SIZE) && pdeg )
  {
    codeGroup = ++countRegGroup;
    arrGroupProc[codeGroup] = pdeg; // ������������ ������
  }
  else
  {
#if defined(TD_WINDOWS)
    ::MessageBox( NULL, "������ ����������� ������ �������", "����������� �������", MB_OK | MB_TASKMODAL );
#else
		char st[100];
    fprintf( stderr, bl_W2L( "����������� �������: ������ ����������� ������ �������", st, sizeof(st) ) );
#endif
  }
  end()
  return codeGroup;
}
//------------------------------------------------------------------------------

// ���������������� �������� �������
// name - �������� ��������� (��������)
// ���������: ���������� � �������� ������������� ��������� ��� NULL
void* err_RegisterSource( const char *name, const char *cp )
{
  if( !ES_Heap.init() )
    return NULL;

  gsize bytes_written;
  gchar* str = g_convert( name, -1, bl_locale, cp, NULL, &bytes_written, NULL );
  if( str )
  {
    gchar* lp = g_string_chunk_insert_len( ES_Heap.heap, str, bytes_written );
    g_free( str );
    return lp;
  }
  return NULL;
}
//-----------------------------------------------------------------------------
// ������ ���������� �������� ��������� ������ (null-��������������� ������)
// ���������: ����� �������� (��� ������������ ����)
int err_GetStrSource( void* source, char* buf, int len )
{
  if( source && len )
  {
    int l = (int)strlen( (char*)source );
    len = ( l >= len ) ? len-1 : l;
    memcpy( buf, (char*)source, len );
    buf[len] = '\0';
  }
  else len = 0;
  return len;
}
//-----------------------------------------------------------------------------
// ������ ���������� �������� ������ �� �� ������ � ���� (null-��������������� ������)
// ���������: ����� ������ � ������ (��� ������������ ����)
int err_GetStrError( guint8 group, size_t code, char* buf, int len )
{
  // ���� ���������� ��� ������
  if( group && group < countRegGroup )
  {
    TGetStrError procGetStr = arrGroupProc[group]->procGetStr;
    // ���� � ���������� ������ ����������� ��� ������� - ...
    if( procGetStr ) return procGetStr( code, buf, len );
  }
  return 0;
}
//------------------------------------------------------------------------------
// �������� ������� ������ - ����� �������
void err_ClearBufError()
{
  // ��� ���� ������������������ ����� ������� �������� ������� ������� �������
  PDefErrorGroup* pDEG = arrGroupProc + 1;
  for( guint8 i = countRegGroup; i > 0; i--, pDEG++ )
    if( *pDEG && (*pDEG)->procClearBuf ) (*pDEG)->procClearBuf();
}
//------------------------------------------------------------------------------

