/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BL_DebugH
#define BL_DebugH

#include "TypeDef.h"

/*
  Warning!
  ������� ��� �� �����(��� ������� ����� ��������, ��� �� ����������):
    BL_ASSERT(Func());
  ���������:
    expression = Func();
    BL_ASSERT(expression);
*/


// ������� �� ����� ��������� �� ������.
SHARE_EI bool BL_MessageBug( const char* lpszMsg );

// ������� �� ����� ��������� �� ������.
SHARE_EI void BL_MessageBug( const char* lpszFileName, int nLine );

#ifdef _DEBUG
  // ��������� ��������� <f> � ������� �� ����� ��������� �� ������ � ������ ��� ��������
  #define BL_ASSERT(f)          (void) ( (f) || (BL_MessageBug(__FILE__, __LINE__),0) )

  // ��������� �������� ����������� ���������� <f>
  #define BL_DEBUG(f)           f

  // ������� �� ����� ��������� �� ������. ���� ��� ��������
  #define BL_FIX_BUG()          BL_MessageBug(__FILE__, __LINE__)

#else
  #define BL_ASSERT(f)          
  #define BL_DEBUG(f)           
  #define BL_FIX_BUG()
#endif  //_DEBUG


#endif  //BL_DebugH

