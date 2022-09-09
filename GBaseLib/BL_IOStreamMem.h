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
