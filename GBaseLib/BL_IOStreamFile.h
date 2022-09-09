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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


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
