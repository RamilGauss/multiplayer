#include <windows.h>
#include <vector>

class TBase
{
protected:
  void* pInheritsThis;
public:
  TBase(){pInheritsThis=NULL;}
  void* GetBaseThis(){return this;}
  void* GetInherits(){return pInheritsThis;}
};

class TA : public TBase
{
  void* pA;
public:
  TA(){pA = this;}
  void* GetAThis(){return pA;}
};

class TB : virtual public TA
{
  void* pB;
public:
  TB(){pB = this;}
  void* GetBThis(){return pB;}
};
class TC : virtual public TA
{
  void* pC;
public:
  TC(){pC = this;}
  void* GetCThis(){return pC;}
};

class TD : public TB, public TC
{
  void* pD;
public:
  TD(){pD = this;}
  void* GetDThis(){return pD;}
};


class TRealize : public TD
{
  void* pR;
public:
  TRealize(){pInheritsThis = this;pR = this;}
  void* GetRThis(){return pR;}

};

INT WINAPI wWinMain( HINSTANCE, HINSTANCE, LPWSTR, int )
{
  /*TRealize d;
  TBase* pBase     = &d; 
  void*  pPtrBase  = pBase->GetBaseThis();
  void*  pInherits = pBase->GetInherits();*/

  return 0;
}
