#include "GlobalParamsTank.h"

#include <stdio.h>


char strVersionClient[100];
char strVersionServer[100];
char strVersionEditor[100];

class TStrVersion
{
public:
  TStrVersion();
};


static TStrVersion strVersion;

//-------------------------------------------------------------------------------------
TStrVersion::TStrVersion()
{
  sprintf(strVersionClient,"������ \"Tanks\" v%0.3f NET DX9",VERSION_PROGRAMM/1000.0f);
  sprintf(strVersionServer,"������ \"Tanks\" v%0.3f NET DX9",VERSION_PROGRAMM/1000.0f);
  sprintf(strVersionEditor,"�������� ������� v%0.3f",VERSION_EDITOR/1000.0f);
}
//-------------------------------------------------------------------------------------