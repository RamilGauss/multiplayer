#include <string.h>
#include "ErrorReg.h"
#include <glib.h>

guint8 egSTD;
//------------------------------------------------------------------------------
static int GetStr( size_t code, char* buf, int len )
{
  char *strErr = strerror( (int)code );
  if( !strErr )
  {
    int l = g_snprintf( buf, len, "Error code %d", code );
    return (l >= len) ? len-1 : l;
  }
  int l = (int)strlen( strErr );
  if( l >= len ) l = len - 1;
  memcpy( buf, strErr, l );
  return l;
}
//------------------------------------------------------------------------------
static TDefErrorGroup deg = {
  GetStr,
  NULL,
  NULL
};
//------------------------------------------------------------------------------
void errSTD_Set( guint32 code, const char* file, int line, void* source )
{
  err_SetError( egSTD, code, file, line, source );
}
//------------------------------------------------------------------------------
// Сохранить код отказа
guint32 errSTD_push()
{
  return errno;
}
//------------------------------------------------------------------------------
// Восстановить код отказа
void errSTD_pop( guint32 errCode )
{
  errno = errCode;
}
//------------------------------------------------------------------------------
bool errSTD_Init(void)
{
  ERR_REGGROUP( egSTD, deg )
}
//------------------------------------------------------------------------------

