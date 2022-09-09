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

#ifndef LoaderTreeH
#define LoaderTreeH


#include "TreeJoint.h"
#include "BL_ConfigFile.h"
#include <d3dx10math.h>
#include "IXML.h"
#include "TypeDef.h"

class SHARE_EI TLoaderTree
{
  TTreeJoint::TLoadedJoint* pLoadedTree;
  
  IXML* mXML;
public:

  TLoaderTree();
  ~TLoaderTree();

  bool Load(char* sPath);
  TTreeJoint::TLoadedJoint* TakeTree();

protected:
  bool LoadMatrix4x4(const char* name,int num, nsStruct3D::TMatrix16/*D3DXMATRIXA16*/* pM);
  bool LoadJoint(int i);
  //bool LoadVector4(TBL_ConfigFile* fileIni,char* strNumPart,char* key,D3DXVECTOR4& vector4);
  //float FindFloat_Semicolon(char** buffer,bool* ok);
  //char* FindSemicolon(char* buffer);

};

#endif