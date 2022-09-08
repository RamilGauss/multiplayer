#include "ErrorReg.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


//-----------------------------------------------------------------------------
#define LEN_DATE  11 // "2005/12/31 10:10:10, "

static int AddDate( tm *ptm, char** pbuf, int &size )
{
  if( size <= LEN_DATE )
  {
    **pbuf = 0;
    return 0;
  }
  int len = sprintf( *pbuf, "%04d/%02d/%02d ",
    1900+ptm->tm_year, 1+ptm->tm_mon, ptm->tm_mday );
  size -= len;
  *pbuf = *pbuf + len;
  return size;
}
//-----------------------------------------------------------------------------
#define LEN_TIME  10 // "10:10:10, "

static int AddTime( tm *ptm, char** pbuf, int &size )
{
  if( size <= LEN_TIME )
  {
    **pbuf = 0;
    return 0;
  }
  int len = sprintf( *pbuf, "%02d:%02d:%02d, ", ptm->tm_hour, ptm->tm_min, ptm->tm_sec );
  size -= len;
  *pbuf = *pbuf + len;
  return size;
}
//-----------------------------------------------------------------------------
static int AddSource( void* source, char** pbuf, int &size )
{
  char* buf = *pbuf;

  // Запрос текстового описания источника отказа
  int len = err_GetStrSource( source, buf, size );
  if( len )
  {
    size -= len;
    buf += len;
    if( size <= 2 ) return 0;
    *buf++ = ':'; *buf++ = ' ';
    size -= 2;
    *pbuf = buf;
  }
  return size;
}
//-----------------------------------------------------------------------------
static int AddText( guint8 group, size_t code, char* *pbuf, int &size )
{
  // Запрос текстового описания отказа по ее группе и коду
  char* buf = *pbuf;
  int len = err_GetStrError( group, code, buf, size );
  if( len )
  {
    *pbuf = buf + len;
    size -= len;
  }
  return size;
}
//-----------------------------------------------------------------------------
static int AddStr( const char* str, char* *pbuf, int &size )
{
  if( !str ) return size;

  char* buf = *pbuf;
	int len = (int)strlen( str );
  if( len >= size ) len = size - 1;
  memcpy( buf, str, len );
  *pbuf = buf + len;
  size -= len;
  if( size > 1 ) return size;
  else
  {
    **pbuf = 0;
    return 0;
  }
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void errDesc_Fill( TErrorDef &def, TErrorDesc &desc )
{
  char *buf = desc.str;
  int size = TErrorDesc::size_str;
  tm* ptm = localtime( &def.time );
  if( ptm )
    AddDate( ptm, &buf, size );
  desc.startTime = TErrorDesc::size_str - size;
  if( size )
  {
    if( ptm )
      AddTime( ptm, &buf, size );
    desc.startSource = TErrorDesc::size_str - size;
    if( size )
    {
      AddSource( def.source, &buf, size );
      if( size )
      {
        AddText( def.group, def.code, &buf, size );
        desc.startFile = TErrorDesc::size_str - size;
        if( size )
        {
          if( !def.file )
            *buf = 0;
          else
          {
			if( AddStr( ", ", &buf, size ) &&
                AddStr( def.file, &buf, size ) &&
#ifdef TD_WINDOWS
                AddStr( ", линия-", &buf, size ) )
#else
                AddStr( ", string-", &buf, size ) )
#endif
            {
              char s[10];
              sprintf( s, "%d", def.line );
              if( AddStr( s, &buf, size ) )
                *buf = 0;
            }
		  }
        }
        goto l_length;
      }
    }
    desc.startFile = TErrorDesc::size_str - size;
  }
  else
    desc.startSource = desc.startFile = TErrorDesc::size_str - size;
l_length:
  desc.length = TErrorDesc::size_str - size;
}
//-----------------------------------------------------------------------------

