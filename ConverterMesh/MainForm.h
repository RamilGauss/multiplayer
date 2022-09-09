/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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


#ifndef MainFormConverter_H
#define MainFormConverter_H

#include "DXUT.h"
#include <QWidget>
#include "ui_MainForm.h"
#include <QList>
#include <QFileInfo>
#include "IMeshFile.h"


class TMainForm : public QWidget
{
  Q_OBJECT

  QFileInfoList mListPath;

  IMeshFile         mMeshFile;
  LPDIRECT3DDEVICE9 m_pd3dDevice;
  LPDIRECT3D9       m_pD3D;

public:
  TMainForm(QWidget *parent = NULL);
  virtual ~TMainForm();


protected slots:

  void sl_SetPath();
  void sl_Start();

protected:
  
  void SaveInReestr();
  void LoadFromReestr();

  void MakeListPath();
  void ConvertAll();
  void Convert(const char* sPath);

  void DestroyDevice();
  void MakeDevice();

private:
  Ui::FormMainForm ui;
};

#endif // EDITORMODEL_H
