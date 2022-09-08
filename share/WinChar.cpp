#include "WinChar.h"



#include <atlconv.h>

//int _convert = 0; //(_convert); 
//UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; //(_acp); 
//LPCWSTR _lpw = NULL; //(_lpw); 
//LPCSTR _lpa = NULL; //(_lpa);


WCHAR* A2W_(char* _lpa)
{
  UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; //(_acp); 

  int _convert = (lstrlenA(_lpa)+1);
  LPWSTR l = (LPWSTR)malloc(_convert*sizeof(WCHAR));
  WCHAR* p = (WCHAR*)ATLA2WHELPER(l , _lpa, _convert, _acp);

  return p;
}