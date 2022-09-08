/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013
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


#ifndef IImageNotifyH
#define IImageNotifyH


#include "UIDobject.h"
//-----------------------------------------------------------------------------
// ������ ���������, ������������ ��������, ������������ ImageFile
#define MFL_COUNT       10

#define MFL_OPEN        0   // ���� ������
#define MFL_CLOSE       1   // ���� ������
#define MFL_RELOAD      2   // ���� ��� ���������� �� ��
#define MFL_INSERT      3   // �������� ������
#define MFL_CHANGE      4   // ������� ������
#define MFL_DELETE      6   // ������ ������

#if MFL_DELETE > MFL_COUNT
  #error MFL_DELETE > MFL_COUNT
#endif
//-----------------------------------------------------------------------------

// ����� �������, ������������ ���� - ��������� ������
#define IFM_OPEN        (1<<0)  // �������� ��������� MFL_OPEN
#define IFM_CLOSE       (1<<1)  // �������� ��������� MFL_CLOSE
#define IFM_INSERT      (1<<2)  // �������� ��������� MFL_INSERT
#define IFM_DELETE      (1<<3)  // �������� ��������� MFL_DELETE
#define IFM_CHANGE      (1<<6)  // �������� ��������� MFL_CHANGE
//.....................
#define IFM_ALL (IFM_OPEN | IFM_CLOSE | IFM_INSERT | IFM_DELETE | IFM_CHANGE)

//=============================================================================
//=============================================================================
class TImageFile;

class IImageNotify
{
protected:
public:
  // ���������� ������� �������� �����
  virtual void OnOpen( TImageFile* ) {}

  // ���������� ������� �������� �����
  virtual void OnClose( TImageFile* ) {}

  // ���������� ������� �������� �������
  // index - �������
  // p     - ��������� �� ������
  virtual void OnDelete( TImageFile*, int , PUIDobject ) {}

  // ���������� ������� �������� ������� ��������
  virtual void OnDeleteArray( TImageFile* ) {}

  // ���������� ������� ��������� �������
  // oldIndex - ������ ������ � �������
  // newIndex - ����� ������ � �������
  // p     - ��������� �� ������
  virtual void OnChange( TImageFile* , int , int , PUIDobject  ) {}

  // ���������� ������� ���������� �������
  // index - �������
  // p     - ��������� �� ������
  virtual void OnInsert( TImageFile* , int , PUIDobject  ) {}
};
//=============================================================================
//=============================================================================

#endif
