/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
===========================================================================
                        Common Information
"Tanks" GPL Source Code

This file is part of the "Tanks" GPL Source Code.

"Tanks" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"Tanks" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "Tanks" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "Tanks" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "Tanks" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef BL_LogH
#define BL_LogH

#include <list>
#include "ILogConsole.h"
#include "BL_Conv.h" 
#include <stdarg.h>

class GBASELIB_EI TLogWriter
{
  friend class TLogManager;
  typedef std::list<ILogConsole*> TListConsole;
  // �������� ���������� ������
  bool checkEnabled_p();
  bool mIsLogThreadID;

public:
  // ������� � ������� ���-������ � ������
  // ��������� ����� �������� � ������ [LOG] ����������������� ����� ���������� (��. ����������� � ini-�����).
  // �� ��������� ������ � ���-���� �������������.
  TLogWriter( const char* key = NULL );
  ~TLogWriter();

  // ������� ���-������ � ��������� �������� ������ ������
  void setConsole( const char* name );

  // ������� ���-������ � ��������� �������� ������ ������
  void setConsole( ILogConsole *console );

  // �������� � ���-������ ����� � ���������� ���������
  void addConsole( ILogConsole *console );
  void addConsole( const TListConsole &list );

  // ������ ��� ���-������� 
  // ��������� ����� �������� � ������ [LOG] ����������������� ����� ���������� (��. ����������� � ini-�����).
  void setName( const char* key );
  const char* name() const { return mName; }
  
  // ����������� �� ���������� �������
  void disableConsole( ILogConsole* console );

  // �������� ������� ��������� �������
  void reset();

public: // ���������� ���������� ������� �������
  enum 
  { 
    tsDate = (1<<0), // ����
    tsTime = (1<<1), // ����� (� ��������� �� ������)
    tsMS   = (1<<2), // �������������� �����
    
    tsDateTime = tsDate | tsTime,
    tsTimeMS   = tsTime | tsMS,
  };
  // ������ ������ ��������� ����� �������
  // ��������: 
  //    tsTime     - ������ ����� 
  //    tsDateTime - ���� � �����
  //    tsTimeMS   - ����� � ��������� �� �����������
  void markTimeStamp( int timestamp );

  // �������� �� ������������� ������, �� �������� ���������� ������� �����������
  void markThreadId( bool flag );

private:
  #ifdef _MSC_VER
    # pragma warning(push)
    # pragma warning( disable : 4251 )  
      TListConsole  mListConsole;
    # pragma warning(pop)
  #else
    TListConsole  mListConsole;
  #endif
  
  /// �������� "��������"
	char* mName;

  /// ���������� ������
	bool mEnable;
  
  /// ���� ������������� �������� ������� �� ����� ������ ����� ������
  bool mEOL;

  /// ���� ������������� �������� ������� �� ����� ������ ����� ������
  int  mTimestamp;

  /// �������� ������ �� ���� ��������
  void write( const char* st, int len = -1 );

  /// �������� ������ � ��������� ������
  int  getStrTime( char* inOutStr );

  /// �������� ������ � ��������������� ������
  int  getStrThreadID( char* inOutStr );

public:
  /// �������� ���������� ������
  bool checkEnabled();

  // ������������� ����� ������ � ���
  void suspend()  { mEnable = false; }
  // ������������ ����� ������ � ���
  void resume()   { mEnable = true; }

  // ���� ������������� �������� ������� �� ����� ������ ����� ������
  // �������� �� ��������� - true
  void setEOL( bool value ) { mEOL = value; }
  bool getEOL()const        { return mEOL; }

  // ����� ������
  // cp - code page (��. BL_Conv.h)
  void printf( const char* cp, const char* format, ... );
  void vprintf( const char* cp, const char* format, va_list arglist );

  // ����� ����� ������ � HEX ������� - �� ����� �������� � UTF-16!!!
  // cp - code page (��. BL_Conv.h)
  // comment - ����������� � �����
  // buf, len - ����� � ������ ����� ������
  void hex( const char* cp, const char* comment, const void* buf, size_t len );

public: // ������� ��� ��������� � ��������� CP1251

  void printfW( const char* format, ... );
  void hexW( const char* comment, const void* buf, size_t len )
  { hex( BL_CP_WIN, comment, buf, len ); }

public: // ������� ��� ��������� � ��������� ANSI
  void printfA( const char* format, ... );
  void hexA( const char* comment, const void* buf, size_t len )
  { hex( bl_locale, comment, buf, len ); }
};
//-------------------------------------------------------------------------
// �������� ���������� ������
inline bool TLogWriter::checkEnabled()
{
  if( !mEnable ) return false;

  if( !mListConsole.empty() )
    return true;
  
  //if( !mName ) return false;

  return checkEnabled_p();
}
//-------------------------------------------------------------------------
inline void TLogWriter::printf( const char* cp, const char* format, ... )
{
  va_list arglist;
  va_start( arglist, format );
  if( checkEnabled() )
    vprintf( cp, format, arglist );
  va_end( arglist );
}
//-----------------------------------------------------------------------------
inline void TLogWriter::printfW( const char* format, ... )
{
  va_list arglist;
  va_start( arglist, format );
  if( checkEnabled() )
    vprintf( BL_CP_WIN, format, arglist );
  va_end( arglist );
}
//-----------------------------------------------------------------------------
inline void TLogWriter::printfA( const char* format, ... )
{
  va_list arglist;
  va_start( arglist, format );
  if( checkEnabled() )
    vprintf( bl_locale, format, arglist );
  va_end( arglist );
}
//-----------------------------------------------------------------------------

typedef TLogWriter TBL_Log;

//-------------------------------------------------------------------------
#define APP_LOG_NAME  "APP"

class GBASELIB_EI TBL_LogApp : public TBL_Log
{
public:
  TBL_LogApp() : TBL_Log( APP_LOG_NAME ) {}
};

// ������ ���-���� ����������
extern GBASELIB_EI TBL_LogApp AppLog;

//-------------------------------------------------------------------------
//
#define BL_Log      AppLog.printfW
#define BL_LogHEX   AppLog.hexW

//-------------------------------------------------------------------------
void GBASELIB_EI BL_LogTo( const char* cp, const char* key, const char* format, va_list arglist );
void GBASELIB_EI BL_LogToA( const char* key, const char* format, ... );
void GBASELIB_EI BL_LogToW( const char* key, const char* format, ... );
//-------------------------------------------------------------------------


#endif // BL_LogH
