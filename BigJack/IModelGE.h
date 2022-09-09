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


#ifndef IModelGEH
#define IModelGEH

#include "Struct3D.h"

#include "TObject.h"
#include <vector>
#include <map>

class TShaderStack;

class IModelGE : public TObject
{
public:
  
  IModelGE(){};
  virtual ~IModelGE(){};
  virtual unsigned int GetID() = 0;
  virtual void SetID(unsigned int id) = 0;

  virtual int GetCntEffect() = 0;

  /*
    Рендер маски частей
    с ориентацией и положением частей
    в мировых координатах
  */
  virtual void Draw(std::vector<unsigned char>* state,     //                           (От ObjectDX)
                    std::vector<unsigned char>* mask,      //                           (От ObjectDX)
                    std::vector<nsStruct3D::TMatrix16*>* matrix,//кол-во совпадает с cSubset (От ObjectDX)
                    nsStruct3D::TMatrix16* mWorld,    // где и как расположен объект         (От ObjectDX)
                    float alphaTransparency,  // прозрачность                        (От ObjectDX)
                    nsStruct3D::TMatrix16* pView) = 0; // расположение и ориентация камеры    (от ManagerDirectX)

  virtual void SortPartByAlphabetic() = 0;

  virtual unsigned int GetIndexVisualGroupByName(char* sName, int num) = 0;
  virtual const char* GetNameByIndex(int index) = 0;
  virtual int GetNumUseByIndex(int index) = 0;

  
  virtual void SetShaderStackMask(std::vector<int>* pVectorMask) = 0;// по этой маске настраиваются шейдеры эффектов
  virtual TShaderStack* GetShaderStack(int index) = 0;// взять стек для настройки

};
//-----------------------------------------------------------------



#endif