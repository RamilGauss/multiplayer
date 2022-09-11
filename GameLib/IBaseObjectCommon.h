/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IBaseObjectCommonH
#define IBaseObjectCommonH

#include "IBaseObjectGE.h"
#include "IBaseObjectPrediction.h"


class IBaseObjectCommon : public IBaseObjectGE, public IBaseObjectPrediction
{

public:
  IBaseObjectCommon();
  virtual ~IBaseObjectCommon();


  // ���������� ��������� ������ �� ������� �� "��� �������"
  // ������ ���� ���������
  virtual bool GetMirror(char** pData, int& size) = 0;
  // ����������� �������
  virtual void SetMirror(char* pData, int size) = 0;

  // ������� �� ������������ ��������
  virtual void SetHuman(char* pData, int size){};// ��� �������� ��������, ����������� �� ���������� �������


  // �� ������!!!!!!!
  // delete []p;
  virtual int GetSizeProperty(){return 0;};
  virtual char* GetProperty(){return NULL;};
  virtual void SetProperty(char* pData,int size){};

};
//-------------------------------------------------------------

#endif