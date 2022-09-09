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
you may contact in writing [ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#include "ManagerModelDX.h"
#include <d3d9.h>
#include "ModelDX.h"
#include "GlobalParams.h"
#include "LoadFromHDD.h"
#include "Logger.h"
#include "BL_Debug.h"
#include "LoaderListPathID.h"
#include <atlconv.h>
#include "file_operation.h"
#include "StorePathResources.h"
#include "MapXML_Field.h"

using namespace std;

TManagerModelDX::TManagerModelDX()
{
  mManagerResourceDX = NULL;
  mD3dDevice = NULL;
  LoadListPath();
}
//--------------------------------------------------------------------------------------
TManagerModelDX::~TManagerModelDX()
{
  Done();
}
//--------------------------------------------------------------------------------------
TModelDX* TManagerModelDX::Load(unsigned int id)
{
  GetLogger()->Get("GE")->WriteF_time("Начало загрузки модели ID=%u\n",id);

  string sPath = mMapPathModel[id];
  TModelDX* pModel = NULL;
  if(sPath.length())
  {
    pModel = new TModelDX(mManagerResourceDX);
    pModel->SetID(id);
    USES_CONVERSION;
    if(pModel->Init(mD3dDevice,A2W(sPath.data()))==false)
    {
      GetLogger()->Get("GE")->WriteF_time("Не удалось загрузить модель sPath=%s, id=%u\n",sPath.data(),id);
      delete pModel;
      return NULL;
    }
    pModel->ResetDevice();
    pModel->SortPartByAlphabetic();// сортировать
    mMapLoadedModel[id] = pModel;
  }
  else
  {
    GetLogger()->Get("GE")->WriteF_time("Не удалось найти модель sPath=%s, id=%u\n",sPath.data(),id);
    BL_FIX_BUG();
  }
  
  GetLogger()->Get("GE")->WriteF_time("Конец загрузки модели ID=%u\n",id);
  return pModel;
}
//--------------------------------------------------------------------------------------
void TManagerModelDX::DestroyModel()
{
  TMapUintPtr::iterator bit = mMapLoadedModel.begin();
  TMapUintPtr::iterator eit = mMapLoadedModel.end();
  while(bit!=eit)
  {
    TModelDX* pModel = (*bit).second;
    if(pModel)
      pModel->Destroy();
    delete pModel;
    bit++;
  }
  mMapLoadedModel.clear();
}
//--------------------------------------------------------------------------------------
void TManagerModelDX::LostDevice()
{
  TMapUintPtr::iterator bit = mMapLoadedModel.begin();
  TMapUintPtr::iterator eit = mMapLoadedModel.end();
  while(bit!=eit)
  {
    TModelDX* pModel = (*bit).second;
    if(pModel)
      pModel->LostDevice();
    bit++;
  }
}
//--------------------------------------------------------------------------------------
void TManagerModelDX::ResetDevice()
{
  TMapUintPtr::iterator bit = mMapLoadedModel.begin();
  TMapUintPtr::iterator eit = mMapLoadedModel.end();
  while(bit!=eit)
  {
    TModelDX* pModel = (*bit).second;
    if(pModel)
      pModel->ResetDevice();
    bit++;
  }
}
//--------------------------------------------------------------------------------------
bool TManagerModelDX::LoadListPath()
{
  TLoaderListPathID loader;
  char sAbsPath[300];
  FindAbsPath((char*)GetStorePathResources()->GetSecond("model"),sAbsPath,sizeof(sAbsPath));
  if(loader.Load(sAbsPath,&mMapPathModel)==false)
  {
    GetLogger()->Get("GE")->WriteF_time("Не удалось загрузить список моделей.\n");
    BL_FIX_BUG();
    return false;
  }
  
  PrepareForDX();

  GetLogger()->Get("GE")->WriteF_time("Список моделей загружен успешно.\n");
  return true;
}
//--------------------------------------------------------------------------------------
void TManagerModelDX::Done()
{
  mMapPathModel.clear();
  DestroyModel();
}
//--------------------------------------------------------------------------------------
TModelDX* TManagerModelDX::Find(unsigned int id)
{
  // 1. либо ищем в списке загруженных
  TModelDX* pFindModel = mMapLoadedModel[id];
  if(pFindModel)
    return pFindModel;
  // 2. либо загружаем
  TModelDX* pLoadedModel = Load(id);
    return pLoadedModel;
}
//--------------------------------------------------------------------------------------
void TManagerModelDX::PrepareForDX()
{
  TMapUintStr::iterator bit = mMapPathModel.begin();
  TMapUintStr::iterator eit = mMapPathModel.end();
  while(bit!=eit)
  {
    (*bit).second += "DX\\main.ini";
    bit++;
  }
}
//--------------------------------------------------------------------------------------
