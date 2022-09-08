#include "Testing.h"


#include <list>
#include "ApplicationProtocolPacketAnswer.h"

class TClass
{
public:
  TClass(int b)
  {
    a = b;
  }
  ~TClass()
  {
    a = -1;
  }
  int a;
};


void Testing(void* p, int* size)
{
  TA_Correct_Packet_State_Tank packet;
  // расчет координат возможен до загрузки карты, все данные хранятся у предсказателя
  int cnt = 2;
  packet.setCountTank(cnt);
  for(int i = 0 ; i < cnt ; i++)
  {
    packet.setID(i,i);
    packet.setX(i,0);
    packet.setY(i,1);  
    packet.setZ(i,2);
    packet.setXV(i,3);
    packet.setYV(i,4);
    packet.setZV(i,5);
  }

  return ;

  std::list<TClass*> l;
  TClass* pClass = new TClass(1);
  l.push_back(pClass);
  pClass = new TClass(2);
  l.push_back(pClass);


  std::list<TClass*>::iterator it = l.begin();
  std::list<TClass*>::iterator eit = l.end();
  while(it!=eit)
  {
    (*it)->a = 10;
    delete *it;
    it++;
  }

  l.clear();

}