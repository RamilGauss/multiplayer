
#include "Win_Func.h"
#include "GlobalParamsTank.h"
#include <windows.h>


void WinMessageBox(char* sError)
{
  MessageBox(NULL,sError,NAME_TITTLE,0);
}


