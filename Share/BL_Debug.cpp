/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#include <stdio.h>
#include <string>
using namespace std;

//------------------------------------------------------------------------------
#if defined(_WIN32)//-----------------------------------------------------------
//------------------------------------------------------------------------------
#include <shlwapi.h>
#include "BL_Debug.h"
#if defined(__BORLANDC__)
	#pragma link "shlwapi.lib"     
#endif

//-----------------------------------------------------------------------------
// ��������� �� ������ ��������������� � ���� ���������
// lpszMsg - ����� ���������
// ������ ������� ������������ �� ��������� - OK - true, Cancel - false
//******************************************************************************
bool BL_MessageBug( const char* lpszMsg )
{
  std::string text;
  text = lpszMsg;
  char caption[MAX_PATH];
  if( ::GetModuleFileNameA( NULL, caption, MAX_PATH ) )
    PathStripPathA( caption );
  else
    lstrcpy( caption, "������������ ����" );

  if( ::MessageBoxA( NULL, text.data(), caption, MB_OKCANCEL | MB_SYSTEMMODAL | MB_TOPMOST ) != IDOK )
    return false;
  return true;
}
//-----------------------------------------------------------------------------
// ��������� �� ������ ��������������� � ���� ���������
// lpszFileName - ��� ����� ��� ������������� ������
// nLine - ����� ������
//******************************************************************************
void BL_MessageBug( const char* lpszFileName, int nLine )
{
  static bool show = true;
  if( show )
  {
    char text[MAX_PATH];
    wsprintf( text, "%s, %d", lpszFileName, nLine );
    if(BL_MessageBug( text )==false)
      show = false;
  }
}
//------------------------------------------------------------------------------
#else //defined(_WIN32)//-------------------------------------------------------
//------------------------------------------------------------------------------

#include "BL_Debug.h"
//#include "BL_Conv.h"
//------------------------------------------------------------------------------
void BL_MessageBug( const char* lpszFileName, int nLine )
{
  fprintf( stderr, "Fix bug: %s, %d\n", lpszFileName, nLine );
  fflush( stderr );
}
//------------------------------------------------------------------------------
#endif //TD_TARGET
//------------------------------------------------------------------------------

