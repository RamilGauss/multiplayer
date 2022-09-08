#include "Testing.h"


#include <list>

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
  std::list<TClass*> l;
  TClass* pClass = new TClass(1);
  l.push_back(pClass);
  pClass = new TClass(2);
  l.push_back(pClass);


  std::list<TClass*>::iterator it = l.begin();
  std::list<TClass*>::iterator eit = l.end();
  while(it!=eit)
  {
    delete *it;
    it++;
  }

  l.clear();

}