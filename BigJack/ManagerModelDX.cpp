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


#include "ManagerModelDX.h"
#include <d3d9.h>
#include "ModelDX.h"
#include "GlobalParamsTank.h"
#include "LoadFromHDD.h"
#include "Logger.h"
#include "BL_Debug.h"
#include "LoaderListPathID.h"
#include <atlconv.h>
#include "file_operation.h"

using namespace std;

TManagerModelDX::TManagerModelDX()
{
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
  GlobalLoggerDX.WriteF_time("Начало загрузки модели ID=%u\n",id);

  string sPath = mMapPathModel[id];
  TModelDX* pModel = NULL;
  if(sPath.length())
  {
    pModel = new TModelDX();
    pModel->SetID(id);
    USES_CONVERSION;
    if(pModel->Init(mD3dDevice,A2W(sPath.data()))==false)
    {
      GlobalLoggerDX.WriteF_time("Не удалось загрузить модель sPath=%s, id=%u\n",sPath.data(),id);
      delete pModel;
      return NULL;
    }
    pModel->ResetDevice();
    pModel->SortPartByAlphabetic();// сортировать
    mMapLoadedModel[id] = pModel;
  }
  else
  {
    GlobalLoggerDX.WriteF_time("Не удалось найти модель sPath=%s, id=%u\n",sPath.data(),id);
    BL_FIX_BUG();
  }
  
  GlobalLoggerDX.WriteF_time("Конец загрузки модели ID=%u\n",id);
  return pModel;
}
//--------------------------------------------------------------------------------------
void TManagerModelDX::DestroyModel()
{
  std::map<unsigned int, TModelDX*>::iterator bit = mMapLoadedModel.begin();
  std::map<unsigned int, TModelDX*>::iterator eit = mMapLoadedModel.end();
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
  std::map<unsigned int, TModelDX*>::iterator bit = mMapLoadedModel.begin();
  std::map<unsigned int, TModelDX*>::iterator eit = mMapLoadedModel.end();
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
  std::map<unsigned int, TModelDX*>::iterator bit = mMapLoadedModel.begin();
  std::map<unsigned int, TModelDX*>::iterator eit = mMapLoadedModel.end();
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
  FindAbsPath(PATH_LIST_MODELS,sAbsPath,sizeof(sAbsPath));
  if(loader.Load(sAbsPath,&mMapPathModel)==false)
  {
    GlobalLoggerDX.WriteF_time("Не удалось загрузить список моделей.\n");
    BL_FIX_BUG();
    return false;
  }
  
  PrepareForDX();

  GlobalLoggerDX.WriteF_time("Список моделей загружен успешно.\n");
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
  std::map<unsigned int, std::string>::iterator bit = mMapPathModel.begin();
  std::map<unsigned int, std::string>::iterator eit = mMapPathModel.end();
  while(bit!=eit)
  {
    (*bit).second += "DX\\main.ini";
    bit++;
  }
}
//--------------------------------------------------------------------------------------
