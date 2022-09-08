#include "LogStd.h"
#include <stdio.h>

//-----------------------------------------------------------------------------
void TLogStdOut::write(const char* str, int , bool eol )
{
  fprintf( stdout, str );
  if( eol )
  {
#if TD_TARGET == TD_LINUX
    fprintf( stdout, "\n", 1 );
#else
    fprintf( stdout, "\r\n", 2 );
#endif
  }
}
//-----------------------------------------------------------------------------
void TLogStdErr::write(const char* str, int , bool eol )
{
  fprintf( stderr, str );
  if( eol )
  {
#if TD_TARGET == TD_LINUX
    fprintf( stderr, "\n", 1 );
#else
    fprintf( stderr, "\r\n", 2 );
#endif
  }
}
//-----------------------------------------------------------------------------
