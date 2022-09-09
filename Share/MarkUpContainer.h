/*
===========================================================================
Author: Gudakov Ramil Sergeevich a.k.a. Gauss
Гудаков Рамиль Сергеевич 
2011, 2012, 2013
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

#ifndef MarkUpContainerH
#define MarkUpContainerH

#include <map>
#include <string>
#include <vector>
#include "TypeDef.h"
#include "Container.h"

/*
    Гибкий контейнер.
    Алгоритм работы:
  1. Разметка внутренней структуры.
  2. Выборка нужной области памяти по имени.
  3. Приведение указателя на память к нужному указателю на объект, который туда копируется.
  
  Пример:
  -------------
  сonst             
  -------------
  count = 2   - заголовок
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
//--------------------------------------------------------------------
  typedef enum
  {
    eConst,    // поле постоянного размера
    eVar,      // массив из повторяющихся структур, в начале поля указано кол-во элементов
    eMarkUp,   // одно из полей является данными, которые можно изменять с помощью TMarkUpContainer
  } eType;
  struct TDescConst
  {
    int size;// байт
  };
  struct TDescVar
  {
    int sizeCnt; // байт
    int sizeVar; // байт 
  };
  struct TDescMarkUp
  {
    int sizeSize;// размер поля "размер", байт
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

  void SetMarkUp(std::vector<TCommonDesc>* pVecSize);// разметка контейнера +

  // ?? возможно будет снижение скорости при работе с контейнером (на поиск в map тратится от 3 до 9 мкс под дебагом)
  // дать именованную область на изменение
  // в случае если область будет типа eVar, то используются последние два параметра,
  // иначе используется только name, а остальные параметры игнорируются
  // counter - шапка в которой содержится кол-во 
  // индекс используется только если type_counter==false
  void* Get(const char* name, bool type_counter = true, int index = 0); // +

  // результат - дать область памяти контейнера
  void* GetPtr() const;// +
  int   GetSize();// +
  
  // попытка создать контейнер из области памяти (обязательно должна быть задана разметка)
  // что-то типа взять область памяти, которую заполнил другой объект и по разметке создать карту разметки
  bool Set(void* p, int size);// +
  
  // освободить память
  void Done();// +
  
  // в случае если поменялось кол-во элементов в области типа eVar
  // привести соответствие разметки памяти к разметке в векторе
  void Update();// +

	// аппендикс - это область памяти в конце. Размер этой области это остаток от общей памяти.
	void  SetDataAppendix(void* p, int size);
	
	void* GetDataAppendix(int& size);
	int GetSizeAppendix();
//--------------------------------------------------------------------
protected:
  struct TDesc_Private
  {
    TCommonDesc c;
    int shift;// смещение до начала памяти, байт
    int cntVar; // действительно только если тип eVar
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

  // если ptr=NULL, то запрашивается размер поля, 
  // в котором еще не проинициализировано значение
  int GetSize(TDesc_Private& desc_p, char* ptr);
  void ZeroTensileField();// обнулить поля, отвечающие за размер и кол-во изменяющих размер полей

  int GetSizeByDesc(TDesc_Private& desc_p);

	void SetSizeAppendix(int v);

	void* GetPtrAppendix();
protected:
	// аппендикс
	int mSizeAppendix;
};

//--------------------------------------------------------------------
//--------------------------------------------------------------------

#endif

