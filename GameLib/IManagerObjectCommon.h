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

#ifndef IManagerObjectCommonH
#define IManagerObjectCommonH

#include "SrcEvent.h"
#include <vector>

class IBaseObjectCommon;
class IMakerObjectCommon;

class IManagerObjectCommon : public TSrcEvent
{
protected:
  std::vector< IBaseObjectCommon* > mVectorObject;

public:

  // ������� ������ �������
  typedef enum
  {
    eBeginLoadMap = 0,
    eLoadingMap10,       // ������ 10% �� ��������� ��������
    eEndLoadMap,
    eStoppedLoadMap,
    //---------------------------
    eErrorLoadMap,
  }tEvent;

  IManagerObjectCommon();
  virtual ~IManagerObjectCommon();

  virtual void Init(IMakerObjectCommon* _pMakerObjectCommon) = 0;
  virtual void Clear() = 0;// ����� ��� �������
  virtual void Done() = 0; // ������ � ���������� ���������
  virtual void LoadMap(unsigned int id_map) = 0;// ��������� ����� � ������������ ������
  virtual int  GetProgressLoadMap() = 0;// �������� �������� �����
  virtual IBaseObjectCommon* CreateObject(unsigned int id_model) = 0;

  void AddObject(IBaseObjectCommon* pObject);// ������ �� ��������
  IBaseObjectCommon* Get(int index);// ������ ������ �� ��������� ������� (� �������� ��� �������)
  int GetCountObject();

};

#endif
