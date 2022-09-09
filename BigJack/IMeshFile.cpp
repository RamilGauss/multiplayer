/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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


#include "IMeshFile.h"
#include "BL_Debug.h"
#include <atlconv.h>

IMeshFile::IMeshFile()
{

}
//-------------------------------------------------------------
IMeshFile::~IMeshFile()
{

}
//-------------------------------------------------------------
ID3DXMesh* IMeshFile::Load(IDirect3DDevice9* pd3dDevice, const char* strFilename)
{
  // определить расширение файла
  // obj или bj
  const char* sFind = strstr(strFilename,".");
  if(sFind==NULL) return NULL;
  sFind++;

  if(strcmp(sFind,"obj")==0)
    return LoadFromObj(pd3dDevice, strFilename);
  else if(strcmp(sFind,"bj")==0)
    return LoadFromBj(pd3dDevice, strFilename);

  return NULL;
}
//-------------------------------------------------------------
ID3DXMesh* IMeshFile::LoadFromObj(IDirect3DDevice9* pd3dDevice, const char* strFilename)
{
  USES_CONVERSION;
  if (mMeshFromObj.Create(pd3dDevice,A2W(strFilename))!= S_OK) 
    return NULL;
  
  ID3DXMesh* pMesh = mMeshFromObj.GetMesh();
  mMeshFromObj.ZeroMesh();// отвязаться

  return pMesh;
}
//-------------------------------------------------------------
ID3DXMesh* IMeshFile::LoadFromBj(IDirect3DDevice9* pd3dDevice, const char* strFilename)
{
  return mMeshFromBj.Load(pd3dDevice, strFilename);
}
//-------------------------------------------------------------
bool IMeshFile::SaveAsBj(const char* strFilename,ID3DXMesh* pMesh)
{
  return mMeshFromBj.Save(strFilename,pMesh);
}
//-------------------------------------------------------------