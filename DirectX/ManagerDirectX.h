#ifndef ManagerDirectXH
#define ManagerDirectXH

class TManagerDirectX
{


  bool flgNeedLoadMap;
  //unsigned int 

  bool flgNeedLoadObjectDX;

public:
  TManagerDirectX();
  ~TManagerDirectX();


  void Work();

protected:

  void Refresh();
  void Calc();
  void Optimize();
  void Render();


};

extern TManagerDirectX GlobalManagerDirectX;

#endif