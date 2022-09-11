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


#ifndef IImageNotifyH
#define IImageNotifyH


#include "UIDobject.h"
//-----------------------------------------------------------------------------
// Номера сообщений, относительно базового, генерируемые ImageFile
#define MFL_COUNT       10

#define MFL_OPEN        0   // Файл открыт
#define MFL_CLOSE       1   // Файл закрыт
#define MFL_RELOAD      2   // Файл был перегружен из БД
#define MFL_INSERT      3   // Добавлен объект
#define MFL_CHANGE      4   // Изменен объект
#define MFL_DELETE      6   // Удален объект

#if MFL_DELETE > MFL_COUNT
  #error MFL_DELETE > MFL_COUNT
#endif
//-----------------------------------------------------------------------------

// Маски событий, интересующих окно - владельца образа
#define IFM_OPEN        (1<<0)  // получать сообщение MFL_OPEN
#define IFM_CLOSE       (1<<1)  // получать сообщение MFL_CLOSE
#define IFM_INSERT      (1<<2)  // получать сообщение MFL_INSERT
#define IFM_DELETE      (1<<3)  // получать сообщение MFL_DELETE
#define IFM_CHANGE      (1<<6)  // получать сообщение MFL_CHANGE
//.....................
#define IFM_ALL (IFM_OPEN | IFM_CLOSE | IFM_INSERT | IFM_DELETE | IFM_CHANGE)

//=============================================================================
//=============================================================================
class TImageFile;

class IImageNotify
{
protected:
public:
  // Обработчик события открытия файла
  virtual void OnOpen( TImageFile* ) {}

  // Обработчик события закрытия файла
  virtual void OnClose( TImageFile* ) {}

  // Обработчик события удаления объекта
  // index - позиция
  // p     - указатель на объект
  virtual void OnDelete( TImageFile*, int , PUIDobject ) {}

  // Обработчик события удаления массива объектов
  virtual void OnDeleteArray( TImageFile* ) {}

  // Обработчик события изменения объекта
  // oldIndex - старый индекс в массиве
  // newIndex - новый индекс в массиве
  // p     - указатель на объект
  virtual void OnChange( TImageFile* , int , int , PUIDobject  ) {}

  // Обработчик события добавления объекта
  // index - позиция
  // p     - указатель на объект
  virtual void OnInsert( TImageFile* , int , PUIDobject  ) {}
};
//=============================================================================
//=============================================================================

#endif
