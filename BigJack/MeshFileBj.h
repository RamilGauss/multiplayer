/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
√удаков –амиль —ергеевич 
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

#ifndef MeshFileBjH
#define MeshFileBjH

#include "DXUT.h"

// бинарный формат хранени€ Mesh
// только вершины (координаты, нормали, текстурные координаты) и индексы
namespace nsBigJack{
  // Vertex format

#pragma pack(push, 1)
  struct VERTEX
  {
    D3DXVECTOR3 position;
    D3DXVECTOR3 normal;
    D3DXVECTOR2 texcoord;
  };
#pragma pack(pop)
}


class TMeshFileBj
{

public:
  
  TMeshFileBj();
  virtual ~TMeshFileBj();

  virtual ID3DXMesh* Load(IDirect3DDevice9* pd3dDevice, const char* strFilename);
  virtual bool Save(const char* strFilename,ID3DXMesh* pMesh);

protected:

  bool LoadBj(const char* strFilename);
  void Done();
  bool FillFile(const char* strBj, ID3DXMesh* pMesh);

  void MakeArrVertex(char* pData, DWORD cnt);
  void MakeArrIndex(char* pData, DWORD cnt);
  void MakeArrAttribute();

  char* pReadData;

  DWORD* mArrIndices;
  DWORD mIndicesCnt;

  nsBigJack::VERTEX* mArrVertices;
  DWORD mVerticesCnt;

  DWORD* mArrAttributes;
  DWORD mAttributesCnt;
};

#endif
