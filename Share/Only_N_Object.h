/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef Only_N_ObjectH
#define Only_N_ObjectH

#pragma warning(disable:4355)

#include "BL_Debug.h"
#include "TypeDef.h"

/*
  ������ �������������:
class TClass{
protected:
  TClass():                      // ������ �� ��������
  TOnly_N_Object(1,NAME_CLASS){};// ������ ���� ������
};

*/


#define NAME_CLASS typeid(this).name()

// Thread Safe
class SHARE_EI TOnly_N_Object
{
public:
  TOnly_N_Object(int maxCntObject, std::string nameClass);
  virtual ~TOnly_N_Object();

};

#endif
