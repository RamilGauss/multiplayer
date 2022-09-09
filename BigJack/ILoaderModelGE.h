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


#ifndef ILoaderObjectDXH
#define ILoaderObjectDXH

#include "Struct3D.h"
#include <vector>

#include "IMeshFile.h"
#include "IXML.h"


class ILoaderModelGE
{
public:
  struct TDefGroup
  {
    std::string  strPathShader;// относительный путь
    std::string  strTechnique;
    std::wstring strTexture;
    std::string  strName;
    int          mNumUse;

    void* pMesh;

    nsStruct3D::TVector3 vAmbient;
    nsStruct3D::TVector3 vDiffuse;
    nsStruct3D::TVector3 vSpecular;

    int  nShininess;

    bool bSpecular;

    unsigned char mTypeLOD;
    bool          mflgNormal;

    TDefGroup()
    {
      pMesh     = NULL;

      nShininess = 0;
      bSpecular  = false;

      mTypeLOD   = 0;
      mflgNormal = true;
    }
  };

protected:
  float mLOD;
  
  std::vector<TDefGroup*> mVectorGroup;

public:
  enum{
    eUndefID = -1,
  };

  ILoaderModelGE();
  virtual ~ILoaderModelGE();

  TDefGroup* GetGroup(int i){return mVectorGroup[i];}
  // после загрузки становятся доступными
  float GetLOD();
  int GetCountGroup();

public:
  // virtual
  virtual bool Load(LPCWSTR strFilenameData);

protected:
  
  void Done();

  void UpPath(char* sPath);

protected:

  char pStrPathShader[MAX_PATH];
  char pStrPathTexture[MAX_PATH];
  char pStrFilenameData[MAX_PATH];
  char pStrPathPrimitive[MAX_PATH];
  char pStrFilenameDataMain[MAX_PATH];

  IMeshFile mMeshFile;
  IXML*     mXML;

protected:
  bool LoadMainFile();
  bool LoadFileResource();
  bool LoadPart(int i);
  bool LoadVector(const char* key,nsStruct3D::TVector3& vector);
  bool LoadVector4(const char* key,nsStruct3D::TVector4& vector4);

  virtual bool SetupMesh(TDefGroup *mArrDefGroup);
  
  virtual void* LoadMesh(const char* strPathPrimitives) = 0;
};


#endif