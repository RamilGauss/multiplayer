/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
������� ������ ��������� 
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

#ifndef MarkUpContainerH
#define MarkUpContainerH

#include <map>
#include <string>
#include <vector>
#include "TypeDef.h"
#include "Container.h"

/*
    ������ ���������.
    �������� ������:
  1. �������� ���������� ���������.
  2. ������� ������ ������� ������ �� �����.
  3. ���������� ��������� �� ������ � ������� ��������� �� ������, ���� ����������.
  
  ������:
  -------------
  �onst
  -------------
  count = 2   - ���������
  var
  var
  -------------
*/

class SHARE_EI TMarkUpContainer
{
  typedef std::map<std::string,int> TMapStrInt;
  
  TMapStrInt mMarkUp;
  TContainer mC;
 
public:
  typedef enum
  {
    eConst,
    eVar,
    eMarkUp,  // ���� �� ����� �������� �������, ������� ����� �������� � ������� TMarkUpContainer
  } eType;
  struct TDescConst
  {
    int size;// ����
  };
  struct TDescVar
  {
    int sizeCnt; // ����
    int sizeVar; // ���� 
  };
  struct TDescMarkUp
  {
    int sizeSize;// ������ ���� "������", ����
  };
  struct TCommonDesc
  {
    eType type;
    std::string name; 
    union
    {
      TDescConst  c;
      TDescVar    v;
      TDescMarkUp m;
    };
  };
  //--------------------------------------------------------------------------------------
  TMarkUpContainer();
  virtual ~TMarkUpContainer();

  void SetMarkUp(std::vector<TCommonDesc>* pVecSize);// �������� ���������� +

  // ?? �������� ����� �������� �������� ��� ������ � ����������� (�� ����� � map �������� �� 3 �� 9 ��� ��� �������)
  // ���� ����������� ������� �� ���������
  // � ������ ���� ������� ����� ���� eVar, �� ������������ ��������� ��� ���������,
  // ����� ������������ ������ name, � ��������� ��������� ������������
  // counter - ����� � ������� ���������� ���-�� 
  // ������ ������������ ������ ���� type_counter==false
  void* Get(const char* name, bool type_counter = true, int index = 0); // +

  // ��������� - ���� ������� ������ ����������
  void* GetPtr();// +
  int   GetSize();// +
  
  // ������� ������� ��������� �� ������� ������ (����������� ������ ���� ������ ��������)
  // ���-�� ���� ����������
  bool Set(void* p, int size);// +
  
  // ���������� ������
  void Done();// +
  
  // � ������ ���� ���������� ���-�� ��������� � ������� ���� eVar
  // �������� ������������ �������� ������ � �������� � �������
  void Update();// +
protected:
  struct TDesc_Private
  {
    TCommonDesc c;
    int shift;// �������� �� ������ ������, ����
    int cntVar; // ������������� ������ ���� ��� eVar
    int sizeMarkUp;
    TDesc_Private()
    {
      cntVar     = 0;
      sizeMarkUp = 0;
    }
  };

  typedef std::vector<TDesc_Private> TVectorDesc;
  TVectorDesc mVectorSection;

private:
  int GetValueBy(char* ptr, int sizeCnt);
  void SetCountBy(char* ptr, int sizeCnt, int v);

  // ���� ptr=NULL, �� ������������� ������ ����, 
  // � ������� ��� �� ������������������� ��������
  int GetSize(TDesc_Private& desc_p, char* ptr);
  void ZeroTensileField();// �������� ����, ���������� �� ������ � ���-�� ���������� ������ �����

  int GetSizeByDesc(TDesc_Private& desc_p);
};

//--------------------------------------------------------------------
//--------------------------------------------------------------------

#endif

