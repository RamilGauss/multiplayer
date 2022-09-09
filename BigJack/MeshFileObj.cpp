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

#include "MeshFileObj.h"
#include "LoadFromHDD.h"
#include "BL_Debug.h"
#include <fstream>
#include <list>
#include <map>

using namespace nsMeshStruct;
using namespace nsStruct3D;
using namespace std;

TMeshFileObj::TMeshFileObj()
{
}
//-----------------------------------------------------------------------------------
TMeshFileObj::~TMeshFileObj()
{

}
//-----------------------------------------------------------------------------------
bool TMeshFileObj::Load(const char* strFilename, 
            VERTEX*&      pVertex, unsigned int& cntV,
            unsigned int*& pIndex, unsigned int& cntI)
{
  typedef map<unsigned int, TVector2> TMapUintVector2;
  typedef map<unsigned int, TVector3> TMapUintVector3;
  typedef map<VERTEX, unsigned int>   TMapVertexUint;
  typedef list<VERTEX>                TListVertex;
  
  typedef TMapUintVector2::iterator TMapUV2_It;
  typedef TMapUintVector3::iterator TMapUV3_It;
  typedef TMapVertexUint::iterator  TMapVU_It;
  typedef TListVertex::iterator     TListV_It;
  
  typedef TMapUintVector2::value_type TMapUV2_v;
  typedef TMapUintVector3::value_type TMapUV3_v;
  typedef TMapVertexUint::value_type  TMapVU_v;

  TMapUintVector3 mapPositions;
  TMapUintVector2 mapTexCoords;
  TMapUintVector3 mapNormals;

  TMapVertexUint mapVertex;
  TListVertex    listVertex;

  // File input
  wchar_t strCommand[256] = {0};
  wifstream InFile( strFilename );
  if( !InFile )
    return false;

  for(;;)
  {
    InFile >> strCommand;
    if( !InFile )
      break;

    if( 0 == wcscmp( strCommand, L"#" ) )
    {
      // Comment
    }
    else if( 0 == wcscmp( strCommand, L"v" ) )
    {
      // Vertex Position
      float x, y, z;
      InFile >> x >> y >> z;
      mapPositions.insert( TMapUV3_v( mapPositions.size(),TVector3( x, y, z ) ) );
    }
    else if( 0 == wcscmp( strCommand, L"vt" ) )
    {
      // Vertex TexCoord
      float u, v;
      InFile >> u >> v;
      mapTexCoords.insert( TMapUV2_v( mapTexCoords.size(),TVector2( u, v ) ) );
    }
    else if( 0 == wcscmp( strCommand, L"vn" ) )
    {
      // Vertex Normal
      float x, y, z;
      InFile >> x >> y >> z;
      mapNormals.insert( TMapUV3_v( mapNormals.size() ,TVector3( x, y, z ) ) );
    }
    else if( 0 == wcscmp( strCommand, L"f" ) )
    {
      // Face
      unsigned int iPosition, iTexCoord, iNormal;
      VERTEX vertex;

      for( unsigned int iFace = 0; iFace < 3; iFace++ )
      {
        InFile >> iPosition;
        vertex.position = mapPositions[ iPosition - 1 ];
        if( '/' == InFile.peek() )
        {
          InFile.ignore();
          if( '/' != InFile.peek() )
          {
            // Optional texture coordinate
            InFile >> iTexCoord;
            vertex.texcoord = mapTexCoords[ iTexCoord - 1 ];
          }
          if( '/' == InFile.peek() )
          {
            InFile.ignore();
            // Optional vertex normal
            InFile >> iNormal;
            vertex.normal = mapNormals[ iNormal - 1 ];
          }
        }
        // проверить на дублирование вершин в массиве
        TMapVU_It fit = mapVertex.find(vertex);
        if(fit==mapVertex.end())
          mapVertex.insert(TMapVU_v(vertex,mapVertex.size()));

        listVertex.push_back(vertex);
      }
    }
    else if( 0 == wcscmp( strCommand, L"mtllib" ) )
    {
      // Material library
    }
    else if( 0 == wcscmp( strCommand, L"usemtl" ) )
    {
      // Material
    }
    else
    {
      // Unimplemented or unrecognized command
    }
    InFile.ignore( 1000, '\n' );
  }

  // Cleanup
  InFile.close();

  // создать 
  cntV    = mapVertex.size();
  pVertex = new VERTEX[cntV];
  cntI    = listVertex.size(); 
  pIndex  = new unsigned int[cntI];
  // найти по вершине индекс
  unsigned int iIndex = 0;
  TListV_It bit = listVertex.begin();
  TListV_It eit = listVertex.end();
  while(bit!=eit)
  {
    TMapVU_It fit = mapVertex.find(*bit);
    if(fit!=mapVertex.end())
    {
      pIndex[iIndex] = fit->second;
      iIndex++;
    }
    else BL_FIX_BUG();
    bit++;
  }
  // копировать вершины
  TMapVU_It mbit = mapVertex.begin();
  TMapVU_It meit = mapVertex.end();
  while(mbit!=meit)
  {
    pVertex[mbit->second] = mbit->first;
    mbit++;
  }

  return true;
}
//-----------------------------------------------------------------------------------
bool TMeshFileObj::Save(const char* strFilename, 
                  VERTEX*       pVertex, unsigned int cntV,
                  unsigned int* pIndex,  unsigned int cntI) 
{
  BL_FIX_BUG();// пока не умею сохран€ть
  return false;
}
//-----------------------------------------------------------------------------------
void TMeshFileObj::Done()
{
}
//-----------------------------------------------------------------------------------
