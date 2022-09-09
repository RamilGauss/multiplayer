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


#include "GlobalParams.h"

#include <stdio.h>

//char strVersionClient[100];
//char strVersionServer[100];
//char strVersionViewerModel[100];
//char strVersionConverterMesh[100];
char strVerGameEngine[260];
char strVerGraphicEngine[260];
char strVerPhysicEngine[260];
char strVerNETEngine[260];

class TStrVersion
{
public:
  TStrVersion();
};


static TStrVersion strVersion;

//-------------------------------------------------------------------------------------
TStrVersion::TStrVersion()
{
  //sprintf(strVersionClient,"Клиент \"%s\" v%0.3f NET DX9",NAME_GAME,VERSION_PROGRAMM/1000.0f);
  //sprintf(strVersionServer,"Сервер \"%s\" v%0.3f NET DX9",NAME_GAME,VERSION_PROGRAMM/1000.0f);
  //sprintf(strVersionViewerModel,"Просмоторщик моделей v%0.3f",VERSION_VIEWER_MODEL/1000.0f);
  //sprintf(strVersionConverterMesh,"Конвертер Obj->Bj v%0.3f",VERSION_CONVERTER_MESH/1000.0f);
  sprintf(strVerGameEngine,   "\"Tornado GameEngine\" v%0.3f, numComponents=%d",
    VER_GAME_ENGINE/1000.0f,COUNT_GAME_ENGINE_MODULES);
  sprintf(strVerGraphicEngine,"\"BigJack\" v%0.3f DX9",VER_GRAPHIC_ENGINE/1000.0f);
  sprintf(strVerPhysicEngine, "\"Robert\" v%0.3f ",VER_PHYSIC_ENGINE/1000.0f);
  sprintf(strVerNETEngine,    "\"Melissa\" v%0.3f NET",VER_NET_ENGINE/1000.0f);
}
//-------------------------------------------------------------------------------------
