///////////////////////////////////////////////////////////
//  LogFile.cpp
//  Implementation of the Class TLogFile
//  Created on:      29-май-2010 19:35:49
//  Original author: KIT
///////////////////////////////////////////////////////////

#include "LogFile.h"
#include <glib.h>
#include <string.h>

//-----------------------------------------------------------------------------
TLogFile::TLogFile(const char* nameFile, bool enable )
{
  mEnable = enable;
  mCheckOpenFile = false;
  mName = g_strdup( nameFile );
}
//-----------------------------------------------------------------------------
TLogFile::~TLogFile()
{
  g_free( mName );
}
//-----------------------------------------------------------------------------
bool TLogFile::enable()
{
  bool res;
  if( !mEnable ) res = false;
  else
  {
    if( mCheckOpenFile ) res = od.isOpen();
    else
    {
      lock();
      if( mCheckOpenFile ) mEnable = od.isOpen();
      else
      {
        mEnable = od.create( mName, false, true, true );
        mCheckOpenFile = true;
      }
      unlock();
      res = mEnable;
    }
  }
  return res;
}
//-----------------------------------------------------------------------------
void TLogFile::write(const char* str, int len, bool eol )
{
  lock();
  if( enable() )
  {    
    if( eol )
      od.writeStringEOL( str, len );
    else
    {
      if( len == -1 )
        len = str ? (int)strlen( str ) : 0;
      od.put( str, len );
    }
    od.flush();    
  }
  unlock();  
}
//-----------------------------------------------------------------------------
const char* TLogFile::name() const 
{
	return mName;
}
//-----------------------------------------------------------------------------
