#ifndef KeyDXH
#define KeyDXH

class TKeyDX
{

public:
  TKeyDX();
  ~TKeyDX();

  void Work(unsigned int nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
};


#endif