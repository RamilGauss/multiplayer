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
