/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef BasePacketApplH
#define BasePacketApplH

#include "MarkUpContainer.h"
//#include "TObject.h"

// ������� �������:
/*
    GetSet(Type,NameParam)
    GetSetCount(TypeCnt, TypeElem, NameParam )
*/
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
#define GetByPtr(Type,NameParam) \
Type* p = (Type*)mC.Get(#NameParam);

#define GetElemByPtr(Type, NameParam, index) \
Type* p = (Type*)mC.Get(#NameParam, false, index);
//----------------------------------------------------------------------------------------
#define GetSet(Type,NameParam) \
void Get##NameParam(Type& v) \
{ \
  GetByPtr(Type,NameParam) \
  v = *p; \
} \
void Set##NameParam(Type& v) \
{ \
  GetByPtr(Type,NameParam) \
  *p = v; \
}
//----------------------------------------------------------------------------------------
#define GetSetCount(TypeCnt, TypeElem, NameParam ) \
void SetCount##NameParam(TypeCnt v) \
{ \
  GetByPtr(TypeCnt,NameParam) \
  *p = v; \
  mC.Update(); \
} \
void GetCount##NameParam(TypeCnt& v) \
{ \
  GetByPtr(TypeCnt,NameParam) \
  v = *p; \
} \
void Set##NameParam(TypeElem& v, TypeCnt index) \
{ \
  GetElemByPtr(TypeElem,NameParam,index) \
  *p = v; \
} \
void Get##NameParam(TypeElem& v, TypeCnt index) \
{ \
  GetElemByPtr(TypeElem,NameParam,index) \
  v = *p; \
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// ����� ������ ���������� ��� �������� � ������ ������� ���������
// ��� ������������:
// 1. ����������������.
// 2. �������������� GetType � Init.
// 3. ���������� ������, � ������� ������� ����� �������� ������ � ����� ������.
// � Init � ������� �������� � mC ��������� ��������� ������.

// ����������� �� TObject ��� ����������� ������������� � hArray
class SHARE_EI TBasePacket// : public TObject
{
protected:

  TMarkUpContainer mC;
 
public:

	TBasePacket();
  virtual ~TBasePacket();

  virtual bool Set(void* p, int size);
  virtual void* Get(int &size);
  
  virtual int GetType(){return 0;}

	void  SetData(void* pData, int size);
	void* GetData(int& size);
	
protected:
  virtual void Init() = 0;

};

#endif

