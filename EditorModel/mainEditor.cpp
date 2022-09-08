#include "DX_main.h"
#include "LoggerDX.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
  InitLoggerDX("Editor");

  TDX dx;
  dx.Work(NULL);

  //dx.Work(NULL);

  return 0;
}