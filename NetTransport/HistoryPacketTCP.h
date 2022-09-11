/*
Author: Gudakov Ramil Sergeevich a.k.a. Gauss 
������� ������ ��������� 
Contacts: [ramil2085@mail.ru, ramil2085@gmail.com]
See for more information License.h.
*/

#ifndef HistoryPacketTCPH
#define HistoryPacketTCPH

#include <map>

#include "ContainerRise.h"
#include "ShareMisc.h"

class THistoryPacketTCP
{
public:
  struct TResult
  {
    TResult(){complete=false;}
    void Set(char* b, int s){buffer=b;size=s;complete=true;}
    bool complete;
    char* buffer;
    int size;
  };

  typedef enum{
    eSearchBegin,
    eSearchSize,
    eSearchEnd,
  }eStatePacket;

  int sizePacket;// �������������� ������ ������
  TContainerRise c;    
  eStatePacket   state;
  THistoryPacketTCP();
  void Clear();

  void Analiz(int& beginPos, TResult& res, int readSize, char* buffer);
protected:
  int SearchBegin(int readSize, char* buffer, int beginPos);
  int SearchSize(int readSize, char* buffer, int beginPos);
  int SearchEnd(int readSize, char* buffer, TResult& res, int beginPos);

};


#endif