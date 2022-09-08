#include "LogerDX.h"

TSaveOnHDD GlobalLoggerDX;

void InitLogerDX(char* sNick)
{
  char nameLogFileDX[260];
  sprintf(nameLogFileDX,".\\DirectX%s.log",sNick);
  GlobalLoggerDX.ReOpen(nameLogFileDX);
  GlobalLoggerDX.SetDebug(false);
  GlobalLoggerDX.WriteF_time("Начало логирования.\n");
}
