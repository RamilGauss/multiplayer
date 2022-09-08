#ifndef __MAPH
#define __MAPH

class TMap
{

  unsigned short mID;// идентификатор карты

public:
  TMap();
  ~TMap();

  unsigned short GetID();
  void SetID(unsigned short val);

};

#endif
