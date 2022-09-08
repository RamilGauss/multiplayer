#include "LoggerDX.h"

TSaveOnHDD GlobalLoggerDX;

void InitLoggerDX(char* sNick)
{
  char nameLogFileDX[260];
  sprintf(nameLogFileDX,".\\DirectX%s.log",sNick);
  GlobalLoggerDX.ReOpen(nameLogFileDX);
  //GlobalLoggerDX.SetDebug(false);
  GlobalLoggerDX.WriteF_time("Начало логирования.\n");
}
