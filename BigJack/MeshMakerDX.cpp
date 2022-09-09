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


#include "MeshMakerDX.h"
#include "BL_Debug.h"
#include <atlconv.h>

using namespace nsMeshStruct;

// vertex declaration
D3DVERTEXELEMENT9 VERTEX_DECL_BJ[] =
{
  { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0},
  { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0},
  { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0},
  D3DDECL_END()
};


TMeshMakerDX::TMeshMakerDX()
{

}
//-------------------------------------------------------------
TMeshMakerDX::~TMeshMakerDX()
{

}
//-------------------------------------------------------------
ID3DXMesh* TMeshMakerDX::Load(IDirect3DDevice9* pd3dDevice, const char* strFilename)
{
  // определить расширение файла
  // obj или bj
  const char* sFind = strstr(strFilename,".");
  if(sFind==NULL) return NULL;
  sFind++;

  if(strcmp(sFind,"obj")==0)
    return LoadFrom(pd3dDevice, strFilename,&mMeshFromObj);
  else if(strcmp(sFind,"bj")==0)
    return LoadFrom(pd3dDevice, strFilename,&mMeshFromBj);

  return NULL;
}
//-------------------------------------------------------------
ID3DXMesh* TMeshMakerDX::LoadFrom(IDirect3DDevice9* pd3dDevice, const char* strFilename, IMeshFile* pMeshFile)
{
  VERTEX* pVertex;
  unsigned int cntV;
  unsigned int* pIndex;
  unsigned int cntI;
  pMeshFile->Load(strFilename,pVertex,cntV,pIndex,cntI);
  // массив атрибутов
  unsigned int attributesCnt = cntI/3;
  unsigned int* pArrAttributes = new unsigned int[attributesCnt];
  memset(pArrAttributes, 0, attributesCnt*sizeof(unsigned int));
  //------------------------------------------------------
  HRESULT hr;
  // Create the encapsulated mesh
  ID3DXMesh* pMesh = NULL;
  V( D3DXCreateMesh( cntI / 3, cntV,
    D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_DECL_BJ,
    pd3dDevice, &pMesh ) );

  // Copy the vertex data
  VERTEX* pVertexMesh;
  V( pMesh->LockVertexBuffer( 0, ( void** )&pVertexMesh ) );
  memcpy( pVertexMesh, pVertex, cntV*sizeof( VERTEX ) );
  pMesh->UnlockVertexBuffer();

  // Copy the index data
  DWORD* pIndexMesh;
  V( pMesh->LockIndexBuffer( 0, ( void** )&pIndexMesh ) );
  memcpy( pIndexMesh, pIndex, cntI * sizeof( unsigned int ) );
  pMesh->UnlockIndexBuffer();

  // Copy the attribute data
  DWORD* pSubset;
  V( pMesh->LockAttributeBuffer( 0, &pSubset ) );
  memcpy( pSubset, pArrAttributes, attributesCnt * sizeof( unsigned int ) );
  pMesh->UnlockAttributeBuffer();

  DWORD* aAdjacency = new DWORD[pMesh->GetNumFaces() * 3];
  if( aAdjacency != NULL )
  {
    V( pMesh->GenerateAdjacency( 1e-6f, aAdjacency ) );
    V( pMesh->OptimizeInplace( D3DXMESHOPT_ATTRSORT | D3DXMESHOPT_VERTEXCACHE, aAdjacency, NULL, NULL, NULL ) );

    SAFE_DELETE_ARRAY( aAdjacency );
  }
  else
    SAFE_RELEASE(pMesh);


  SAFE_DELETE_ARRAY(pVertex);
  SAFE_DELETE_ARRAY(pIndex);
  SAFE_DELETE_ARRAY(pArrAttributes);
  
  return pMesh;
}
//-------------------------------------------------------------
bool TMeshMakerDX::SaveAsBj(const char* strFilename,ID3DXMesh* pMesh)
{
  VERTEX* pVertex      = NULL;
  unsigned int cntV    = pMesh->GetNumVertices();
  unsigned int* pIndex = NULL;
  unsigned int cntI    = pMesh->GetNumFaces()*3;

  HRESULT hr;

  V( pMesh->LockVertexBuffer( 0, ( void** )&pVertex ) );
  V( pMesh->LockIndexBuffer( 0, ( void** )&pIndex ) );

  bool res = mMeshFromBj.Save(strFilename, pVertex, cntV, pIndex, cntI);
  
  pMesh->UnlockVertexBuffer();
  pMesh->UnlockIndexBuffer();
  
  return res;
}
//-------------------------------------------------------------