/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IManagerStateMachineH
#define IManagerStateMachineH

#include <string>

class IManagerStateMachine
{

public:

  IManagerStateMachine(){};
  virtual ~IManagerStateMachine(){};

  enum{eAll=-1};

  // ��������� � ������� id ��� �������
  virtual bool Load(const char* sPath, unsigned int& id) = 0;
  // �� id �������� ��������� � �������� �� �����
  virtual bool SetState(const char* sNewStateName, unsigned int id = eAll) = 0;
  // �� id �������� ������� �� k (��������� �� ��������), ������� � ����� �������� ������
  virtual bool GetValue(unsigned int id, unsigned int k, unsigned int& v) = 0;
  virtual bool GetComment(unsigned int id, unsigned int k, std::string&  comment) = 0;


};


#endif