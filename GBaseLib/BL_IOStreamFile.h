#ifndef BL_IOStreamFileH
#define BL_IOStreamFileH

#include "BL_IOStream.h"

/*
      TIOStreamFile - ���������� ������������ ������ TIOStream ������������� � ������.
  ����� ������ ������ ����������� � ��������� ���������.

      TIOStreamFile ������/��������� ���� � ��������� ����������� ������ ������ 1. 
  ������ ref() � unref() ����� �������������� ��� ���������� � ���������� ���������� ������ 
  ��������������. ����� ���������� ������ �������� 0, ���� �����������.
*/

class GBASELIB_EI TIOStreamFile : public TIOStream
{
public:
  TIOStreamFile(void);
  virtual ~TIOStreamFile(void) { close(); }

  // ���������� �����
  int handle() const { return mFile; }
public:

  // ������� ����
  // fname - �������� ����� � ��������� ���������
  // read  - ���������� �� ������ �� �����
  // write - ���������� �� ������ � ����
  // append- ������ � ���� � ������ "������ ���������� � �����"
  bool create( const char* fname, bool read = false, bool write = true, bool append = false );

  // ������� ���� ��� ������� �����
  // fname - �������� ����� � ��������� ���������
  // read  - ���������� �� ������ �� �����
  // write - ���������� �� ������ � ����
  // append- ������ � ���� � ������ "������ ���������� � �����"
  bool open( const char* fname, bool read = true, bool write = true, bool append = false );

  // ������� ������������ ����
  // fname - �������� ����� � ��������� ���������
  // read  - ���������� �� ������ �� �����
  // write - ���������� �� ������ � ����
  // append- ������ � ���� � ������ "������ ���������� � �����"
  bool openExist( const char* fname, bool read = true, bool write = false, bool append = false );

  // ������������ �������� ��������� �����������
  int dup();

  // ������� ������ � ������ ����� ����������
  void link( int handle );

  // ������� ���� (����������� �� ����)
  void close();
  
  // ������� ����
  bool deleteFile();

  const char* nameFile() const { return mNameFile; }
  void setNameFile( const char* nameFile );

public: // ���������� ���������� TIOStream

  // ���������� ���������� �������
  virtual void* errSource() const;
  virtual void setErrSource( void* es ) { mErrSource = es; }
  
  // ������/������ ������
  virtual int read( void* buf, guint size );        // ��������� ������ �� ������
  virtual int write( const void* buf, guint size ); // �������� ������ � �����

  // ��������� �� ������
  virtual long curPos( );             // ������� ������� ��������� ��� -1
  virtual bool goTop( );              // ��������� � ������ �����
  virtual bool goPos( long pos );     // ��������� � �������� �������
  virtual bool goBottom( );           // ��������� � ����� �����
  virtual bool shiftPos( long offset );// �������� ���������

  // ������ ������
  virtual bool isOpen() const;          // ������ �� ������ � ������
  virtual long length();                // ����� �����
  virtual bool flush();                 // �������� ������ � �����
  virtual bool chsize( long size );     // ���������� ������ �����

protected:
  int   mFile;        // ���������� �����
  mutable void* mErrSource;
  char mNameFile[MAX_PATH];
};

#ifndef TD_WINDOWS
  // ���������� ������ �����
  long GBASELIB_EI filelength( int fd );
#endif


#endif // IOStreamFileH
