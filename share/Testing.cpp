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
  TA_End_Fight a;
  a.setMsg("Привет :))");

  char s[400];
  a.getMsg(s);
  int ab = 0;

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