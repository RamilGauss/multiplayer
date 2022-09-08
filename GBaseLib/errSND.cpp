// Linux platform: ALSA API
#include "TypeDef.h"
#include "ErrorReg.h"
#include <string.h>
#include <alsa/error.h>
#include <glib.h>

//#include <sys/soundcard.h>
//#include <alsa/asoundlib.h>
//#include <alsa/pcm.h>


static guint8 egSND;

//------------------------------------------------------------------------------
static int GetStr( size_t code, char* buf, int len )
{
  const char *strErr = snd_strerror( (int)code );
  if( !strErr )
  {
    g_snprintf( buf, len, "Error code %d", code );
    return strlen( buf );
  }
  int l = strlen( strErr );
  len = ( l >= len ) ? len-1 : l;
  memcpy( buf, strErr, len );
  return len;
}
//------------------------------------------------------------------------------
static TDefErrorGroup deg = {
  GetStr,
  NULL,
  NULL
};
//------------------------------------------------------------------------------
void errSND_Set( guint32 code, const char* file, int line, void* source )
{
  err_SetError( egSND, code, file, line, source );
}
//------------------------------------------------------------------------------
bool errSND_Init(void)
{
  if( !egSND )
    egSND = err_RegisterGroup( &deg );

  return egSND;
}
//------------------------------------------------------------------------------
