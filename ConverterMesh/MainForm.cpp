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

#include "MainForm.h"
#include "GlobalParamsTank.h"
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include "BL_Debug.h"


TMainForm::TMainForm(QWidget *parent)
: QWidget(parent)
{
  m_pd3dDevice = NULL;
  m_pD3D       = NULL;

  ui.setupUi(this);

  setWindowTitle(tr(STR_VERSION_CONVERTER_MESH));

  LoadFromReestr();

  connect(ui.bSetPath,SIGNAL(clicked()),SLOT(sl_SetPath()));
  connect(ui.bStart,  SIGNAL(clicked()),SLOT(sl_Start()));
}
//---------------------------------------------------------------------------------------------
TMainForm::~TMainForm()
{
  SaveInReestr();
}
//---------------------------------------------------------------------------------------------
void TMainForm::sl_SetPath()
{
  QString path = ui.lePath->text();
  QFileDialog fileDialog;
  if(!path.isEmpty())
    fileDialog.setDirectory(path);
  fileDialog.setFileMode(QFileDialog::DirectoryOnly);

  if(fileDialog.exec())
    ui.lePath->setText(fileDialog.directory().absolutePath());

  SaveInReestr();
}
//---------------------------------------------------------------------------------------------
void TMainForm::sl_Start()
{
  // для загрузки Mesh нужно устройство
  MakeDevice();
  // создать список файлов
  MakeListPath();
  // загружать по одному и сохранять
  ConvertAll();

  DestroyDevice();
}
//---------------------------------------------------------------------------------------------
void TMainForm::SaveInReestr()
{
  QString path = ui.lePath->text();

  QSettings settings("RUSSIA","ConverterMesh");
  settings.setValue("path", path);
}
//-----------------------------------------------------------------
void TMainForm::LoadFromReestr()
{
  QSettings settings("RUSSIA","ConverterMesh");
  QString path = settings.value("path", QString()).toString();
  ui.lePath->setText(path);
}
//---------------------------------------------------------------------------------------------
void TMainForm::MakeListPath()
{
  mListPath.clear();
  // сканируем установленную директорию
  // ищем все что содержит расширение "obj"
  QDir dir(ui.lePath->text());
  QStringList nameFilters;
  nameFilters << tr("*.obj");
  mListPath = dir.entryInfoList(nameFilters, QDir::Files );
}
//---------------------------------------------------------------------------------------------
void TMainForm::ConvertAll()
{
  int cnt = mListPath.size();
  for(int i = 0 ; i < cnt ; i++)
    Convert(mListPath[i].filePath().toLocal8Bit().data());
}
//---------------------------------------------------------------------------------------------
void TMainForm::Convert(const char* sPath)
{
  ID3DXMesh* pMesh = mMeshFile.Load(m_pd3dDevice,sPath);
  BL_ASSERT(pMesh);
  mMeshFile.SaveAsBj(sPath,pMesh);
  SAFE_RELEASE(pMesh);
}
//---------------------------------------------------------------------------------------------
void TMainForm::DestroyDevice()
{
  SAFE_RELEASE(m_pd3dDevice) ;
  SAFE_RELEASE(m_pD3D);
}
//---------------------------------------------------------------------------------------------
void TMainForm::MakeDevice()
{
  // Создание объекта Direct3D
  m_pD3D = Direct3DCreate9( D3D_SDK_VERSION);
  if( !m_pD3D )
    BL_FIX_BUG();

  // Создание устройства рендера.
  D3DPRESENT_PARAMETERS d3dpp = {0};
  d3dpp.Windowed = TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
  d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
  d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  HRESULT hr;
  hr = m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, winId(),
    D3DCREATE_SOFTWARE_VERTEXPROCESSING,
    &d3dpp, &m_pd3dDevice );
  if( FAILED(hr) || !m_pd3dDevice)
    BL_FIX_BUG();
}
//---------------------------------------------------------------------------------------------
