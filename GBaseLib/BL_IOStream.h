/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012
===========================================================================
                        Common Information
"TornadoEngine" GPL Source Code

This file is part of the "TornadoEngine" GPL Source Code.

"TornadoEngine" Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

"TornadoEngine" Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with "TornadoEngine" Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the "TornadoEngine" Source Code is also subject to certain additional terms. 
You should have received a copy of these additional terms immediately following 
the terms and conditions of the GNU General Public License which accompanied
the "TornadoEngine" Source Code.  If not, please request a copy in writing from at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef BL_IOStreamH
#define BL_IOStreamH

#include "TypeDef.h"
#include <stdarg.h>


// ��������� ���������� ������� �����/������
class GBASELIB_EI TIOStream
{
public:
  TIOStream(void);
  virtual ~TIOStream(void) { }


  //.......................................................
public: // ���������� ���������� �������
  //.......................................................
  
  // ����������� �������� �������
  virtual void* errSource() const  { return NULL; }

  // ������ �������� ��������� �������
  virtual void setErrSource( void* ) { }

  //.......................................................
public: // ������/������ ������
  //.......................................................

  // ��������� ������ �� ������
  // buf - �������� �����
  // size - ������ ������
  // ���������: ����� ����������� ������ ��� -1 � ������ ������
  virtual int read( void* buf, guint size ) = 0;

  // �������� ������ � �����
  // buf - �������� �����
  // size - ����� ������
  // ���������: ����� ������ ���������� ������ ��� -1 � ������ ������
  virtual int write( const void* buf, guint size ) = 0;

  // ��������� ������ �� ������
  // buf - �������� �����
  // size - ������ ������
  // ���������: ������ ��������� � ������ ������
  bool get( void* buf, guint size );

  // �������� ������ � �����
  // buf - �������� �����
  // size - ����� ������
  // ���������: ������ �������� � ������ ������
  bool put( const void* buf, guint size );

  //.......................................................
public: // ������/������ ������
  //.......................................................
  bool put( signed char val )     { return put( &val, sizeof(val) ); }
  bool put( unsigned char val )   { return put( &val, sizeof(val) ); }
  bool put( signed short val )    { return put( &val, sizeof(val) ); }
  bool put( unsigned short val )  { return put( &val, sizeof(val) ); }
  bool put( signed int val )      { return put( &val, sizeof(val) ); }
  bool put( unsigned int val )    { return put( &val, sizeof(val) ); }
  bool put( signed long val )     { return put( &val, sizeof(val) ); }
  bool put( unsigned long val )   { return put( &val, sizeof(val) ); }
  bool put( gint64 val )          { return put( &val, sizeof(val) ); }
  bool put( guint64 val )         { return put( &val, sizeof(val) ); }
  bool put( float val )           { return put( &val, sizeof(val) ); }
  bool put( double val )          { return put( &val, sizeof(val) ); }
  bool put( bool val )            { return put( &val, sizeof(char) ); }


  bool get( signed char &val )    { return get( &val, sizeof(val) ); }
  bool get( unsigned char &val )  { return get( &val, sizeof(val) ); }
  bool get( signed short &val )   { return get( &val, sizeof(val) ); }
  bool get( unsigned short &val ) { return get( &val, sizeof(val) ); }
  bool get( signed int &val )     { return get( &val, sizeof(val) ); }
  bool get( unsigned int &val )   { return get( &val, sizeof(val) ); }
  bool get( signed long &val )    { return get( &val, sizeof(val) ); }
  bool get( unsigned long &val )  { return get( &val, sizeof(val) ); }
  bool get( gint64 &val )         { return get( &val, sizeof(val) ); }
  bool get( guint64 &val)         { return get( &val, sizeof(val) ); }
  bool get( float &val )          { return get( &val, sizeof(val) ); }
  bool get( double &val )         { return get( &val, sizeof(val) ); }
  bool get( bool &val )           { val = false; return get( &val, sizeof(char) ); }

  //.......................................................
public: // ��������� ����� 
  //.......................................................

  // �������� ������ � �����
  // ����� ������: <�����><������>
  // st - ����������� ������
  // len - ����� ������ (�� ����� 0xFFFF). ���� len = -1, �� ����������� �������������.
  // ���������: ������ �������� � ������ ������
  bool writeStr( const char* st, int len = -1 );

  // ��������� ������ �� ������
  // buf - ����� ��� ������ (������ ���� ���������� ��� ���������� ������)
  // sizebuf - ������ ������
  // ���������: ������ ������ ��� -1 � ������ ������
  int readStr( char *buf, unsigned sizebuf );

  // ��������.��������� ������ � ����� ��� � writeStr/readStrS, �� ����� ������ �� ����� 0xFF
  bool writeStrS( const char* st, int len = -1 );
  int readStrS( char *buf, unsigned sizebuf );

  // ��������� ������ �� ������
  // ���������: ����������� � ����������� � ������������ ������ ������. ����������� ������ �������� g_free
  gchar* readStr();

  // ����� � ����� ������ � ���������� � ����� �������� �������� ������ 
  // ��� Win32 - "\r\n", ����� - "\n"
  bool writeStringEOL( const char* st, int len = -1 );

  // ����� � ����� ��������������� ������
  bool printf( const char* format, ... );
  bool vprintf( const char* format, va_list arglist );

  //.......................................................
public: // ��������� �� ������
  //.......................................................

  virtual long curPos( );             // ������� ������� ��������� ��� -1
  virtual bool goTop( );              // ��������� � ������ �����
  virtual bool goPos( long pos );     // ��������� � �������� �������
  virtual bool goBottom( );           // ��������� � ����� �����
  virtual bool shiftPos( long delta );// �������� ���������

  //.......................................................
public: // ���������� ���������� �������� � ������
  //.......................................................
  //TIOStream* ref();             // �������� ������ �� �����
  //void unref();                 // ��������� ������ �� �����
  //void close() { unref(); }     // ������� �����

  //// ���������� ������ �������� ����� ��� ������ ���-�� ������ � 0 (�� ��������� = true)
  //bool closeOnUnref();
  //void closeOnUnref( bool flag );

  //.......................................................
public: // ������ ������
  //.......................................................
  virtual bool isOpen() const;      // ������ �� ������ � ������
  virtual long length();            // ����� �����
  virtual bool flush();             // �������� ������ � �����
  virtual bool chsize( long size ); // ���������� ������ �����

protected:
};

//---------------------------------------------------------------

// ����� � ����� ��������������� ������
inline bool TIOStream::printf( const char* format, ... )
{
  va_list arglist;
  va_start( arglist, format );
  bool res = vprintf( format, arglist );
  va_end( arglist );
  return res;
}

//---------------------------------------------------------------

#endif // IOStreamH
