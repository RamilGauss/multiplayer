#ifndef BL_IOStreamMemH
#define BL_IOStreamMemH

#include "BL_IOStream.h"

class GBASELIB_EI TIOStreamMem : public TIOStream
{
public:
  TIOStreamMem(void);
  virtual ~TIOStreamMem(void) { close(); }

  // ������� ���� � ������ �������������� �������
  // file  - ����
  // sizefile - ������ ����� 
  TIOStreamMem( void* file, long sizefile );

  // ������� ���� � ������ �������������� �������
  // file  - ����
  // sizefile - ������ ����� 
  bool Open( void* file, long sizefile );

  // ������� ���� � ������
  // sizebuf - ������ ������ 
  // buffer  - ������� ����� (��� �������� ������� �� ������������)
  //           ���� ������� ����� �� ������������, �� ��������� �������� ��������������
  // canResized  - ���������� ����������� ��������� ������ ������ 
  //           (������ � ������ ������ � ����������� �������)
  bool Create( long sizebuf = 0, void* buffer = NULL, bool canResized = true );

  // ������� ����
  void close();
  
  // ������ ������ � ������ ����� � ������
  void* Ptr() { return mIsOpen ? mBuf : NULL; }

public: // ���������� ���������� TIOStream

  virtual void* errSource() const       { return mErrSource; }
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
  virtual bool chsize( long size );     // ���������� ������ �����

protected:
  bool    mIsOpen;    // ������� �������� �����
  char*   mBuf;       // �����
  long    mFileSize;  // ������ �����
  long    mBufSize;   // ������ ������ ( mBufSize >= mFileSize )
  bool    mSelfBuf;   // ������� ������������ ������ ��� �������� �������
  bool    mCanResized;// ���������� ����������� ������ ������
  //long    mDelta;     // ��� ���������� �����

  long    mPos;       // �������� �������� ��������� �� ������ �����
  void*   mErrSource;

};

#endif // IOStreamMemH
