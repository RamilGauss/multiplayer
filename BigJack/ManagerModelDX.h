/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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


#ifndef ManagerModelH
#define ManagerModelH

#include <map>

#include "DXUT.h"

class TModelDX;
class TManagerResourceDX;

class TManagerModelDX
{

protected:
  IDirect3DDevice9* mD3dDevice;

  TManagerResourceDX* mManagerResourceDX;// отдать модели для получения ресурсов

public:
  TManagerModelDX();
  ~TManagerModelDX();

  void SetManagerResourceDX(TManagerResourceDX* managerResourceDX){mManagerResourceDX=managerResourceDX;}

  bool LoadListPath();

  void Setup(IDirect3DDevice9* pd3dDevice){mD3dDevice = pd3dDevice;};

  TModelDX* Find(unsigned int id);
  // список примитивов (оригиналы, которые используются в ObjectDX)
  TModelDX* Load(unsigned int id);

  void DestroyModel();
  void LostDevice();
  void ResetDevice();


/*
// при создании модели вызывать:
void ResetDevice();


порядок вызова при окончании использования
void OnLostDevice();
void OnDestroyDevice();

*/

protected:

  typedef std::map<unsigned int, std::string> TMapUintStr;
  typedef std::map<unsigned int, TModelDX*>   TMapUintPtr;

  TMapUintStr mMapPathModel;
  TMapUintPtr mMapLoadedModel;

  void Done();

  void PrepareForDX();

};

#endif