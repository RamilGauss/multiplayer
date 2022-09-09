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


#ifndef SaveOnHDDH
#define SaveOnHDDH


#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <malloc.h>
#include <atlconv.h>
#include <list>
#include <vector>
#include <deque>
#include "TypeDef.h"

class SHARE_EI TSaveOnHDD
{

	char sPath[260];

	FILE* pFile;

  bool flgPrintf;
  bool flgDebug;
  bool flgBuffer;

  struct TBuffer
  {
    char* p;
    int size;
    TBuffer()
    {
      p = NULL;
      size = 0;
    }
    ~TBuffer()
    {
      delete[]p;p = NULL;
      size = 0;
    }
  };

  typedef std::vector< unsigned char > TVectorUchar;
  typedef std::vector< TVectorUchar > TVectorVectorUchar;

  TVectorVectorUchar mVectorBuffer;

public:

	TSaveOnHDD(char* path = NULL);
	virtual ~TSaveOnHDD();

	virtual bool ReOpen(char* path, bool append = false );

	virtual bool IsOpen();

	virtual void Write(void* buffer, int size);
  virtual void WriteF(const char* format, ... );
  virtual void WriteF_time(const char* format, ... );

	virtual void Close();

  void SetPrintf(bool val){flgPrintf=val;};
  bool GetPrintf(){return flgPrintf;};

  void SetDebug(bool val){flgDebug=val;};
  bool GetDebug(){return flgDebug;};

  void SetBufferization(bool val){flgBuffer=val;};
  bool GetBufferization(){return flgBuffer;};

protected:
  void Write_Time();

  void ClearBuffer();
  void FlushBuffer();
  void FlushInBuffer(char* buffer, int size);
};


#endif

