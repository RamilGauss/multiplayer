#include "ErrorReg.h"
#include <windows.h>

guint8 egSDK;
//------------------------------------------------------------------------------
static int GetStr( size_t code, char* buf, int len )
{
  len = ::FormatMessage(
      FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL,
      (DWORD)code,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      buf,
      len,
      NULL );
  if( len )
  {
    if( len > 2 )
      while( (buf[len-1] == 0x0D) || (buf[len-1] == 0x0A) )
        buf[--len] = 0;
  }
  return len;
}
//------------------------------------------------------------------------------
static TDefErrorGroup deg = {
  GetStr,
  NULL,
  NULL
};
//------------------------------------------------------------------------------
void errSDK_Set( guint32 code, const char* file, int line, void* source )
{
  err_SetError( egSDK, code, file, line, source );
}
//------------------------------------------------------------------------------
guint32 errSDK_push()
{
  return GetLastError();
}
//------------------------------------------------------------------------------
void errSDK_pop( guint32 errCode )
{
  SetLastError( errCode );
}
//------------------------------------------------------------------------------
bool errSDK_Init(void)
{
  ERR_REGGROUP( egSDK, deg )
}
//------------------------------------------------------------------------------

