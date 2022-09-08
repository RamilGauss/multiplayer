/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
2011, 2012, 2013, 2013, 2013
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
you may contact in writing [ramil2085@mail.ru, ramil2085@mail.ru, ramil2085@mail.ru, ramil2085@gmail.com].
===========================================================================
*/ 


#ifndef IActorH
#define IActorH

#include "IBaseObjectCommon.h"
#include <string>

class TClient;
class TRoom;

//------------------------------------------------------------------------------
/*
  �������� �������� �����. ������������ ������ ��� ������� � ��.
*/
class IActor : public IBaseObjectCommon
{

public:
  IActor();
  virtual ~IActor();
//--------------------------------------------------------------------------------
  // �� IBaseObjectCommon
  virtual bool GetMirror(char** pData, int& size){return false;}
  virtual void SetMirror(char* pData, int size){}

  //virtual int GetSizeProperty();
  //virtual char* GetProperty();
  //virtual void SetProperty(char* pData,int size);
//--------------------------------------------------------------------------------
  // IActor ���������
  void SetType(unsigned int id);
  unsigned int GetType();
//--------------------------------------------------------------------------------
public:
  // ��������� ��� �������
  TRoom* pRoom;

  void SetMasterClient(TClient* _pClient);
  TClient* GetMasterClient();

  // ��� Room, � ���
  unsigned char mGroup;// ����� ������� � �������. ��� 0 ��� 1.

protected:
  TClient* pMasterClient;
//--------------------------------------------------------------------------------
  // ��� �������
  std::string sClientName;
public:
  const char* GetClientName(){return sClientName.data();}
  void        SetClientName(char* str){sClientName.insert(0,str);}
//--------------------------------------------------------------------------------

};
//-----------------------------------------------------------------------------


#endif

