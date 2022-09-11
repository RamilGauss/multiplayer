/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef IManagerTimeH
#define IManagerTimeH

class IManagerTime
{
public:
  IManagerTime(){};
  virtual ~IManagerTime(){};

  // ���������� ������� ��������
  virtual void SetTimeSpeed(float relative = 1.0f) = 0;// ��������� ��������� � ��������
  virtual void SetTimeToBegin() = 0;
  virtual void SetTimeToEnd() = 0;
  virtual int  GetCountTimeStamp() = 0;
  virtual void SetTimeStamp(int stamp) = 0;
  virtual unsigned int GetTime() = 0;
};

#endif
