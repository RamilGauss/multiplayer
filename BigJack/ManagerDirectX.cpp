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
the "Tanks" Source Code.  If not, please request a copy in writing from id Software at the address below.
===========================================================================
                                  Contacts
If you have questions concerning this license or the applicable additional terms,
you may contact in writing [ramil2085@gmail.com].
===========================================================================
*/ 


#include "ManagerDirectX.h"
#include "HiTimer.h"
#include "Logger.h"
#include "BaseObjectDX.h"


#define LOG_DX
//#define LOG_DX_STREAM

using namespace nsStruct3D;

TManagerDirectX::TManagerDirectX()
{
  mD3DDevice = NULL;
}
//--------------------------------------------------------------------------------------------------------
TManagerDirectX::~TManagerDirectX()
{

}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::VisualEvent( guint32 iTime, float fElapsedTime )
{
  Optimize();
  Render(iTime, fElapsedTime);
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Optimize()
{
  mListReadyRender = mListAllObject;

  // собственно сам процесс оптимизации (подсказка: процесс оптимизации заключается в выкидывании лишних
  // не рисуемых и не задействованных объектов, используются BB моделей объектов)
  /*...*/

}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Render( guint32 fTime, float fElapsedTime )
{
  HRESULT hr;
  D3DXMATRIXA16 mView;
  D3DXMATRIXA16 mProj;

  // Clear the render target and the zbuffer 
  V( mD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 0, 45, 50, 170 ), 1.0f, 0 ) );

  mProj = *mCamera.GetProjMatrix();
  mView = *mCamera.GetViewMatrix();

  // Render the scene
  if( SUCCEEDED( mD3DDevice->BeginScene() ) )
  {
    std::list<TBaseObjectDX*>::iterator it = mListReadyRender.begin();
    std::list<TBaseObjectDX*>::iterator eit = mListReadyRender.end();
    while(it!=eit)
    {
      (*it)->Draw(&mView,&mProj);
      it++;
    }
    V( mD3DDevice->EndScene() );
  }
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::CreateDeviceEvent(IDirect3DDevice9* pd3dDevice)
{
  mD3DDevice = pd3dDevice;
  mManagerModel.Setup(mD3DDevice);

  // Setup the camera's view parameters
  D3DXVECTOR3 vecEye( 0.0f, 0.0f, -5.0f );
  D3DXVECTOR3 vecAt ( 0.0f, 0.0f, -0.0f );

  //D3DXVECTOR3 vecEye( 5.0f, 5.0f, 5.0f );
  //D3DXVECTOR3 vecAt ( -10.0f, -10.0f, -10.0f );

  mCamera.SetViewParams( &vecEye, &vecAt );
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::AddObject(TBaseObjectDX* pObject)
{
  TModelDX* pModel = mManagerModel.Find(pObject->GetID_Model());
  if(pModel)
  {
    if(pObject->GetModel()==NULL)
    {
      pObject->SetModel(pModel);
      mListAllObject.push_back(pObject);
    }
    else
    {
      GlobalLoggerDX.WriteF_time("Повторная инициализация объекта сцены, PTR_OBJECT=0x%X\n",pObject);
      BL_FIX_BUG();
    }
  }
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::SetViewParams(D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt)// расположение камеры
{
  mCamera.SetViewParams(pvEyePt, pvLookatPt);
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::Clear()
{
  mListAllObject.clear();
}
//--------------------------------------------------------------------------------------------------------
void TManagerDirectX::SetEffect(unsigned short id_effect/*уникальный эффект, см. таблицу эффектов*/,
               D3DVECTOR& coord,     // где
               D3DVECTOR& orient,    // ориентация эффекта
               float time_past/*// прошло времени, мс*/)
{

}
//--------------------------------------------------------------------------------------------------------
