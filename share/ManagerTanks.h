#ifndef ManagerTanksH
#define ManagerTanksH

class TTank;

class TManagerTanks 
{

public:
  TManagerTanks();

  void LoadPropertyFromIni(TTank* pTank);

protected:
  void Load(char* path,TTank* pTank);

};

extern TManagerTanks GlobalManagerTanks;

#endif