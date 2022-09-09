/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
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

#include "MeshFileBj.h"
#include "SaveOnHDD.h"
#include "LoadFromHDD.h"

using namespace nsBigJack;

// vertex declaration
D3DVERTEXELEMENT9 VERTEX_DECL_BJ[] =
{
  { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_POSITION, 0},
  { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_NORMAL,   0},
  { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,  D3DDECLUSAGE_TEXCOORD, 0},
  D3DDECL_END()
};


TMeshFileBj::TMeshFileBj()
{
  pReadData = NULL;
  mArrIndices = NULL;
  mIndicesCnt = 0;

  mArrVertices = NULL;
  mVerticesCnt = 0;

  mArrAttributes = NULL;
  mAttributesCnt = 0;
}
//-----------------------------------------------------------------------------------
TMeshFileBj::~TMeshFileBj()
{

}
//-----------------------------------------------------------------------------------
ID3DXMesh* TMeshFileBj::Load(IDirect3DDevice9* pd3dDevice, const char* strFilename)
{
  Done();
  if(LoadBj(strFilename)==false) return NULL;

  HRESULT hr;
  // Create the encapsulated mesh
  ID3DXMesh* pMesh = NULL;
  V( D3DXCreateMesh( mIndicesCnt / 3, mVerticesCnt,
    D3DXMESH_MANAGED | D3DXMESH_32BIT, VERTEX_DECL_BJ,
    pd3dDevice, &pMesh ) );

  // Copy the vertex data
  VERTEX* pVertex;
  V( pMesh->LockVertexBuffer( 0, ( void** )&pVertex ) );
  memcpy( pVertex, mArrVertices, mVerticesCnt*sizeof( VERTEX ) );
  pMesh->UnlockVertexBuffer();

  // Copy the index data
  DWORD* pIndex;
  V( pMesh->LockIndexBuffer( 0, ( void** )&pIndex ) );
  memcpy( pIndex, mArrIndices, mIndicesCnt * sizeof( DWORD ) );
  pMesh->UnlockIndexBuffer();

  // Copy the attribute data
  DWORD* pSubset;
  V( pMesh->LockAttributeBuffer( 0, &pSubset ) );
  memcpy( pSubset, mArrAttributes, mAttributesCnt * sizeof( DWORD ) );
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

  Done();

  return pMesh;
}
//-----------------------------------------------------------------------------------
bool TMeshFileBj::Save(const char* strFilename,ID3DXMesh* pMesh)
{
  // определить формат
  DWORD bPerV = pMesh->GetNumBytesPerVertex();
  if(bPerV!= sizeof(VERTEX)) return false;


  // задать путь и имя к файлу типа *.bj
  char* sFind = strstr((char*)strFilename,".");
  int len = int(sFind-strFilename);
  char strBj[300];
  strncpy(strBj,strFilename,len);
  strBj[len] = '\0';
  strcat(strBj,".bj");

  return FillFile(strBj,pMesh);
}
//-----------------------------------------------------------------------------------
bool TMeshFileBj::LoadBj(const char* strFilename)
{
  /*
  4 байта - число точек (DWORD)
   точки по 32 байта
  все остальное - это индексы

  */

  TLoadFromHDD loader;
  if(loader.ReOpen((char*)strFilename)==false) return false;
  int size = loader.Size();

  pReadData = new char[size];
  loader.Read(pReadData,size);

  DWORD cntV = *((DWORD*)pReadData);
  MakeArrVertex(pReadData+sizeof(DWORD),cntV);
  DWORD cntI = (size - sizeof(DWORD)-cntV*sizeof(VERTEX))/sizeof(DWORD);
  MakeArrIndex(pReadData+sizeof(DWORD)+cntV*sizeof(VERTEX),cntI);

  MakeArrAttribute();

  return true;
}
//-----------------------------------------------------------------------------------
void TMeshFileBj::Done()
{
  mArrIndices = NULL;
  mIndicesCnt = 0;

  mArrVertices = NULL;
  mVerticesCnt = 0;

  delete[]mArrAttributes;
  mArrAttributes = NULL;
  mAttributesCnt = 0;
  
  delete[] pReadData;
  pReadData = NULL;
}
//-----------------------------------------------------------------------------------
bool TMeshFileBj::FillFile(const char* strBj, ID3DXMesh* pMesh)
{
  // определить размер данных
  DWORD cntV = pMesh->GetNumVertices();
  DWORD cntI = pMesh->GetNumFaces()*3;

  TSaveOnHDD saver;
  saver.ReOpen((char*)strBj);

  saver.Write(&cntV,sizeof(cntV));// кол-во точек
  // сохранить вершины
  HRESULT hr;
  VERTEX* pVertex;
  V( pMesh->LockVertexBuffer( 0, ( void** )&pVertex ) );
  if(hr==S_OK)
  {
    saver.Write( pVertex, cntV*sizeof( VERTEX ) );
    pMesh->UnlockVertexBuffer();
  }
  else return false;
  // сохранить индексы
  DWORD* pIndex;
  V( pMesh->LockIndexBuffer( 0, ( void** )&pIndex ) );
  if(hr==S_OK)
  {
    saver.Write( pIndex, cntI*sizeof( DWORD ) );
    pMesh->UnlockIndexBuffer();
  }
  else return false;

  return true;
}
//-----------------------------------------------------------------------------------
void TMeshFileBj::MakeArrVertex(char* pData, DWORD cnt)
{
  mVerticesCnt = cnt;
  mArrVertices = (VERTEX*)pData;
}
//-----------------------------------------------------------------------------------
void TMeshFileBj::MakeArrIndex(char* pData, DWORD cnt)
{
  mIndicesCnt = cnt;
  mArrIndices = (DWORD*)pData;
}
//-----------------------------------------------------------------------------------
void TMeshFileBj::MakeArrAttribute()
{
  mAttributesCnt = mIndicesCnt/3;
  mArrAttributes = new DWORD[mAttributesCnt];
  memset(mArrAttributes,0,mAttributesCnt*sizeof(DWORD));
}
//-----------------------------------------------------------------------------------