
#define UNICODE

#include <atlconv.h>
#include "Win_Func.h"
#include "GlobalParamsTank.h"


void WinMessageBox(char* sError)
{
  int _convert = 0;
  UINT _acp = ATL::_AtlGetConversionACP();
  LPCWSTR _lpw = NULL; 
  LPCSTR _lpa = NULL; 

  MessageBox(NULL,A2W(sError),A2W(NAME_TITTLE),0);
}


