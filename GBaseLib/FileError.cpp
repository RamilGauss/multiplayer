#include "FileError.h"
#include <string.h>

//-----------------------------------------------------------------------------
TFileError::TFileError( const char *_name )
{
  errSource = NULL;
  if( _name )
    SetFileName( _name );
  else
    name[0] = '\0';
}
//-----------------------------------------------------------------------------
void TFileError::SetFileName( const char *_name )
{
  if( _name ) strcpy( name, _name );
  else        name[0] = '\0';
}
//-----------------------------------------------------------------------------
void TFileError::SetError( guint8 group, guint32 code, const char* file, int line )
{
  if( !errSource && name[0] ) errSource =  err_RegisterSource( name ); 
  err_SetError( group, code, file, line, errSource );
}
//-----------------------------------------------------------------------------
void TFileError::SetErrorStr( const char* cp, const char* text, const char* file, int line )
{
  if( !errSource && name[0] ) errSource = err_RegisterSource( name );
  errStr_Set( cp, text, file, line, errSource );
}
//-----------------------------------------------------------------------------
void TFileError::SetErrorSTD( guint32 code, const char* file, int line )
{
  if( !errSource && name[0] ) errSource =  err_RegisterSource( name );
  errSTD_Set( code, file, line, errSource );
}
//-----------------------------------------------------------------------------
